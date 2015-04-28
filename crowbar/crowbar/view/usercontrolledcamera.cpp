#include "usercontrolledcamera.h"
#include <QTime>

VIEW_BEGIN_NAMESPACE

UserControlledCamera::UserControlledCamera(QObject *parent) :
    GLAngleCamera(parent)
{
    init();
}

void UserControlledCamera::init()
{
    m_vecVelocity = QVector3D(0,0,0);
    m_iMovementFlags = NoMovement;
    m_flTopSpeed = 1000.0f;
    m_flAccelTime = 0.5f;
    m_flDecelTime = 0.2f;
}

float UserControlledCamera::topSpeed() const
{
    return m_flTopSpeed;
}

void UserControlledCamera::setTopSpeed(float speed)
{
    m_flTopSpeed = speed;
}

float UserControlledCamera::accelTime() const
{
    return m_flAccelTime;
}

void UserControlledCamera::setAccelTime(float seconds)
{
    m_flAccelTime = seconds;
}

float UserControlledCamera::decelTime() const
{
    return m_flDecelTime;
}

void UserControlledCamera::setDecelTime(float seconds)
{
    m_flDecelTime = seconds;
}

QVector3D UserControlledCamera::velocity() const
{
    return m_vecVelocity;
}

void UserControlledCamera::setVelocity(const QVector3D &velocity)
{
    setVelocityNoClamp(clampLength(velocity, topSpeed()));
}

void UserControlledCamera::setVelocityNoClamp(const QVector3D &velocity)
{
    m_vecVelocity = velocity;
}

void UserControlledCamera::clearMovementFlags()
{
    m_iMovementFlags = NoMovement;
}

void UserControlledCamera::forward(bool pressed)
{
    if ( pressed ) m_iMovementFlags |= MoveForward;
    else m_iMovementFlags &= ~MoveForward;
}

void UserControlledCamera::backward(bool pressed)
{
    if ( pressed ) m_iMovementFlags |= MoveBackward;
    else m_iMovementFlags &= ~MoveBackward;
}

void UserControlledCamera::left(bool pressed)
{
    if ( pressed ) m_iMovementFlags |= MoveLeft;
    else m_iMovementFlags &= ~MoveLeft;
}

void UserControlledCamera::right(bool pressed)
{
    if ( pressed ) m_iMovementFlags |= MoveRight;
    else m_iMovementFlags &= ~MoveRight;
}

void UserControlledCamera::up(bool pressed)
{
    if ( pressed ) m_iMovementFlags |= MoveUp;
    else m_iMovementFlags &= ~MoveUp;
}

void UserControlledCamera::down(bool pressed)
{
    if ( pressed ) m_iMovementFlags |= MoveDown;
    else m_iMovementFlags &= ~MoveDown;
}

QVector3D UserControlledCamera::vectorFromButtons(MovementState buttons)
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    if ( ((buttons & MoveRight) == MoveRight) && ((buttons & MoveLeft) != MoveLeft) ) x = 1.0f;
    else if ( ((buttons & MoveLeft) == MoveLeft) && ((buttons & MoveRight) != MoveRight) ) x = -1.0f;

    if ( ((buttons & MoveForward) == MoveForward) && ((buttons & MoveBackward) != MoveBackward) ) y = 1.0f;
    else if ( ((buttons & MoveBackward) == MoveBackward) && ((buttons & MoveForward) != MoveForward) ) y = -1.0;

    if ( ((buttons & MoveUp) == MoveUp) && ((buttons & MoveDown) != MoveDown) ) z = 1.0f;
    else if ( ((buttons & MoveDown) == MoveDown) && ((buttons & MoveUp) != MoveUp) ) z = -1.0f;

    return QVector3D(x, y, z).normalized();
}

void UserControlledCamera::updateFromVelocity()
{
    // TODO: We should probably make this a proper member timer so that we can disable it if we need to.
    // Otherwise, if this function isn't called for a while (eg. if the view sleeps), acc/dec could accumulate.
    static QTime time;
    static bool initialised = false;

    if ( !initialised )
    {
        time.start();
        initialised = true;
    }
    
    // If the time elapsed is 0 miliseconds, don't do anything.
    // Otherwise our acceleration/deceleration doesn't work.
    if ( time.elapsed() < 1 ) return;

    // Time elapsed since last call:
    int ms = time.restart();

    // In any axes where no buttons are pressed (ie. where the axis value is zero), we want to decelerate back towards zero speed.
    // In axes where buttons are pressed, we want to accelerate in this direction.

    // Fraction of a second passed:
    float frac = (float)ms/1000.0f;

    // Calculate the acceleration and deceleration that should occur over the time that has elapsed.
    float dec = decelTime() <= 0.0f ? 0.0f : ((topSpeed()/decelTime()) * frac);
    float acc = accelTime() <= 0.0f ? 0.0f : ((topSpeed()/accelTime()) * frac);

    // Get the direction in which we should accelerate.
    // X = left/right as defined by the cross product of view direction and (0,0,1).
    // Y = forward/backward.
    // Z = up/down in world space.
    QVector3D buttonDir = vectorFromButtons(m_iMovementFlags);

    // Decelerate back towards zero in directions in which no buttons are pressed.
    // Make sure we also account for if dec is 0 - this means that we should stop immediately.
    if ( buttonDir.x() == 0.0f )
    {
        if ( m_vecVelocity.x() > 0.0f )
        {
            m_vecVelocity.setX(dec > 0.0f ? qMax(m_vecVelocity.x() - dec, 0.0f) : 0.0f);
        }
        else if ( m_vecVelocity.x() < 0.0f )
        {
            m_vecVelocity.setX(dec > 0.0f ? qMin(m_vecVelocity.x() + dec, 0.0f) : 0.0f);
        }
    }

    if ( buttonDir.y() == 0.0f )
    {
        if ( m_vecVelocity.y() > 0.0f )
        {
            m_vecVelocity.setY(dec > 0.0f ? qMax(m_vecVelocity.y() - dec, 0.0f) : 0.0f);
        }
        else if ( m_vecVelocity.y() < 0.0f )
        {
            m_vecVelocity.setY(dec > 0.0f ? qMin(m_vecVelocity.y() + dec, 0.0f) : 0.0f);
        }
    }

    if ( buttonDir.z() == 0.0f )
    {
        if ( m_vecVelocity.z() > 0.0f )
        {
            m_vecVelocity.setZ(dec > 0.0f ? qMax(m_vecVelocity.z() - dec, 0.0f) : 0.0f);
        }
        else if ( m_vecVelocity.z() < 0.0f )
        {
            m_vecVelocity.setZ(dec > 0.0f ? qMin(m_vecVelocity.z() + dec, 0.0f) : 0.0f);
        }
    }
    
    // TODO: For better accuracy when moving around, if a button is pressed and the camera is moving in the opposite direction (eg.
    // if left is pressed but we're still moving right), negate the direction in that axis instead of decelerating and then accelerating.

    // Add the motion vector from the buttons that are pressed to the current speed.
    // If acc is zero, just set velocity to the max speed.
    setVelocity(velocity() + (buttonDir * (acc > 0.0f ? acc : topSpeed())));

    // Modify our position depending on our current velocity.
    translateCameraLocal(velocity() * frac);
}

void UserControlledCamera::translateCameraLocal(const QVector3D translation)
{
    // Calculate the translation in world co-ordinates.
    // X = right or left, taking the up vector to be (0,0,1).
    // Y = forward or backward.
    // Z = up or down in world space.

    QVector3D right = QVector3D::crossProduct(viewVector(), QVector3D(0,0,1));
    if ( !right.isNull() ) right.normalize();

    QVector3D apply = (translation.x() * right) + (translation.y() * viewVector()) + (translation.z() * QVector3D(0,0,1));

    setEye(eye() + apply);
    setCenter(center() + apply);
}

VIEW_END_NAMESPACE
