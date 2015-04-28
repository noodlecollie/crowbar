#include "qglcamera2.h"
#include <QTime>

QGLCamera2::QGLCamera2(QObject *parent) :
    QGLCamera(parent)
{
    init();
}

void QGLCamera2::init()
{
    m_vecVelocity = QVector3D(0,0,0);
    m_iMovementFlags = NoMovement;
    m_flTopSpeed = 32.0f;
    m_flAccelTime = 0.2f;
    m_flDecelTime = 0.2f;
    m_flMaxPitchDelta = 89.0f;
    m_flDefEyeCenterDist = distanceFromEyeToCenter();
}

float QGLCamera2::topSpeed() const
{
    return m_flTopSpeed;
}

void QGLCamera2::setTopSpeed(float speed)
{
    m_flTopSpeed = speed;
}

float QGLCamera2::accelTime() const
{
    return m_flAccelTime;
}

void QGLCamera2::setAccelTime(float seconds)
{
    m_flAccelTime = seconds;
}

float QGLCamera2::decelTime() const
{
    return m_flDecelTime;
}

void QGLCamera2::setDecelTime(float seconds)
{
    m_flDecelTime = seconds;
}

QVector3D QGLCamera2::velocity() const
{
    return m_vecVelocity;
}

// Clamps the velocity to the max speed.
void QGLCamera2::setVelocity(const QVector3D &velocity)
{
    setVelocityNoClamp(clampLength(velocity, topSpeed()));
}

void QGLCamera2::setVelocityNoClamp(const QVector3D &velocity)
{
    m_vecVelocity = velocity;
}

void QGLCamera2::clearMovementFlags()
{
    m_iMovementFlags = NoMovement;
}

void QGLCamera2::forward(bool pressed)
{
    if ( pressed ) m_iMovementFlags |= MoveForward;
    else m_iMovementFlags &= ~MoveForward;
}

void QGLCamera2::backward(bool pressed)
{
    if ( pressed ) m_iMovementFlags |= MoveBackward;
    else m_iMovementFlags &= ~MoveBackward;
}

void QGLCamera2::left(bool pressed)
{
    if ( pressed ) m_iMovementFlags |= MoveLeft;
    else m_iMovementFlags &= ~MoveLeft;
}

void QGLCamera2::right(bool pressed)
{
    if ( pressed ) m_iMovementFlags |= MoveRight;
    else m_iMovementFlags &= ~MoveRight;
}

void QGLCamera2::up(bool pressed)
{
    if ( pressed ) m_iMovementFlags |= MoveUp;
    else m_iMovementFlags &= ~MoveUp;
}

void QGLCamera2::down(bool pressed)
{
    if ( pressed ) m_iMovementFlags |= MoveDown;
    else m_iMovementFlags &= ~MoveDown;
}

void QGLCamera2::addPitch(float f)
{
    setViewAngles(viewAngles() + QAngle(f,0.0f,0.0f));
    //viewAngleClamp(f < 0);
    simpleViewAngleClamp();
}

void QGLCamera2::addYaw(float f)
{
    setViewAngles(viewAngles() + QAngle(0.0f,f,0.0f));
}

void QGLCamera2::addRoll(float f)
{
    setViewAngles(viewAngles() + QAngle(0.0f,0.0f,f));
}

QAngle QGLCamera2::viewAngles() const
{
    return QAngle::vectorToAngle(fromCameraSpace(viewVector()));
}

void QGLCamera2::setViewAngles(const QAngle &angles)
{
    setViewVector(angles.toCameraVector());
}

float QGLCamera2::maxPitchDelta() const
{
    return m_flMaxPitchDelta;
}

void QGLCamera2::setMaxPitchDelta(float delta)
{
    if ( delta < 0.0f ) delta = 0.0f;
    m_flMaxPitchDelta = delta;
    simpleViewAngleClamp();
}

void QGLCamera2::simpleViewAngleClamp()
{
    QAngle angles = viewAngles();
    viewAngleClamp(angles.pitch() >= 180.0f);
}

void QGLCamera2::viewAngleClamp(bool rotateUpwards)
{
    QAngle angles = viewAngles();
    
    // Clamp if outside the range.
    if ( angles.pitch() < 360.0f - maxPitchDelta() && angles.pitch() > maxPitchDelta() )
    {
        angles.setPitch(rotateUpwards ? 360.0f - maxPitchDelta() : maxPitchDelta());
        setViewAngles(angles);
    }
}

QVector3D QGLCamera2::vectorFromButtons(MovementState buttons)
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    
    if ( ((buttons & MoveRight) == MoveRight) && ((buttons & MoveLeft) != MoveLeft) ) x = 1.0f;
    else if ( ((buttons & MoveLeft) == MoveLeft) && ((buttons & MoveRight) != MoveRight) ) x = -1.0f;
    
    if ( ((buttons & MoveUp) == MoveUp) && ((buttons & MoveDown) != MoveDown) ) y = 1.0f;
    else if ( ((buttons & MoveDown) == MoveDown) && ((buttons & MoveUp) != MoveUp) ) y = -1.0f;
    
    if ( ((buttons & MoveForward) == MoveForward) && ((buttons & MoveBackward) != MoveBackward) ) z = 1.0f;
    else if ( ((buttons & MoveBackward) == MoveBackward) && ((buttons & MoveForward) != MoveForward) ) z = -1.0;
    
    return QVector3D(x, y, z).normalized();
}

void QGLCamera2::updateFromVelocity()
{
    static QTime time;
    static bool initialised = false;
    
    if ( !initialised )
    {
        time.start();
        initialised = true;
    }
    
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
    
    // Add the motion vector from the buttons that are pressed to the current speed.
    // If acc is zero, just set velocity to the max speed.
    setVelocity(velocity() + (buttonDir * (acc > 0.0f ? acc : topSpeed())));
    
    // Modify our position depending on our current velocity.
    translateCamera(velocity() * frac);
}

float QGLCamera2::defaultEyeCenterDistance() const
{
    return m_flDefEyeCenterDist;
}

void QGLCamera2::setDefaultEyeCenterDistance(float f)
{
    m_flDefEyeCenterDist = f;
}

void QGLCamera2::resetDefaultEyeCenterDistance()
{
    setDefaultEyeCenterDistance(DEFAULT_E_C_DIST);
}
