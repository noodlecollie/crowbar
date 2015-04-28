#include "cameracontrollerv2.h"
#include "qglcamera2.h"

CameraControllerV2::CameraControllerV2(QObject *parent) :
    QObject(parent)
{
    init();
}

void CameraControllerV2::init()
{
    m_bEnabled = false;
    m_pCamera = NULL;
    m_iCurrentState = 0;
    m_vecVelocity = QVector3D(0,0,0);
    m_qViewAngles = QAngle(0,0,0);
    
    // Set defaults.
    m_flTopSpeed = 8.0f;
    m_flAccelTime = 0.0f;
    m_flDecelTime = 0.0f;
}

void CameraControllerV2::setCamera(QGLCamera *camera)
{
    if ( m_pCamera != camera )
    {
        if ( m_pCamera ) disconnect(m_pCamera.data(), SIGNAL(viewChanged()), this, SLOT(cameraChanged()));
        if ( camera ) connect(camera, SIGNAL(viewChanged()), this, SLOT(cameraChanged()));
    }
    
    m_pCamera = camera;
    cameraChanged();
}

QGLCamera* CameraControllerV2::camera() const
{
    return m_pCamera;
}

void CameraControllerV2::processMovement(int dMsec)
{
    if ( !m_bEnabled )
    {
        m_vecVelocity = QVector3D(0,0,0);
        return;
    }
    
    // If the dt is 0, just return and wait for the next call.
    if ( dMsec < 1 ) return;
    
    if ( camera() )
    {
        // Calculate the new velocity vector
        calculateVelocityVector(dMsec);
        
        // Apply the velocity translation to the camera, proportional to the time that has passed.
        applyCameraTranslation(dMsec);
    }
}

void CameraControllerV2::calculateVelocityVector(int msec)
{
    // Calculate the direction of movement from the buttons.
    QVector3D buttonDir = vectorFromButtons(m_iCurrentState);
    //if ( !buttonDir.isNull() ) qDebug() << "Button direction:" << buttonDir;
    
    // In any axes where no buttons are pressed (ie. where the axis value is zero), we want to decelerate back towards zero speed.
    // In axes where buttons are pressed, we want to accelerate in this direction.
    
    float frac = (float)msec/1000.0f;
    float dec = decelerationTime() <= 0.0f ? 0.0f : ((topSpeed()/decelerationTime()) * frac);
    float acc = accelerationTime() <= 0.0f ? 0.0f : ((topSpeed()/accelerationTime()) * frac);
    
    if ( buttonDir.x() == 0.0f )
    {
        if ( m_vecVelocity.x() > 0.0f ) m_vecVelocity.setX(qMax(m_vecVelocity.x() - dec, 0.0f));
        else if ( m_vecVelocity.x() < 0.0f ) m_vecVelocity.setX(qMin(m_vecVelocity.x() + dec, 0.0f));
    }
    
    if ( buttonDir.y() == 0.0f )
    {
        if ( m_vecVelocity.y() > 0.0f ) m_vecVelocity.setY(qMax(m_vecVelocity.y() - dec, 0.0f));
        else if ( m_vecVelocity.y() < 0.0f ) m_vecVelocity.setY(qMin(m_vecVelocity.y() + dec, 0.0f));
    }
    
    if ( buttonDir.z() == 0.0f )
    {
        if ( m_vecVelocity.z() > 0.0f ) m_vecVelocity.setZ(qMax(m_vecVelocity.z() - dec, 0.0f));
        else if ( m_vecVelocity.z() < 0.0f ) m_vecVelocity.setZ(qMin(m_vecVelocity.z() + dec, 0.0f));
    }
    
    // Add the motion vector from the buttons to the current speed.
    // If acc is zero, just set velocity to the max speed.
    if ( acc == 0.0f ) m_vecVelocity = buttonDir.normalized() * topSpeed();
    else m_vecVelocity += (buttonDir * acc);
    
    // Clamp speed to max.
    if ( m_vecVelocity.length() > topSpeed() )
    {
        m_vecVelocity *= (topSpeed()/m_vecVelocity.length());
    }
}

// Assumes camera is valid.
void CameraControllerV2::applyCameraTranslation(int msec)
{
    // Since the velocity vector is in units per second, we need to multiply the distances by the
    // fraction of the second that has passed;
    float frac = (float)msec/1000.0f;
    
    // Is camera->translation() required here? Can we just do translateEye()/translateCenter() with the current vector, considering
    // it's local velocity anyway?
    // Note that if we do camera->translation(), Z is applied in the inverse direction - not sure why.
    //QVector3D translation = m_pCamera->translation(m_vecVelocity.x() * frac, m_vecVelocity.y() * frac, m_vecVelocity.z() * frac);
    
    QVector3D translation = m_vecVelocity * frac;
    //if ( !translation.isNull() ) qDebug() << "Velocity:" << m_vecVelocity << "Translation:" << translation;
    
    m_pCamera->translateCenter(translation.x(), translation.y(), translation.z());
    m_pCamera->translateEye(translation.x(), translation.y(), translation.z());
}

float CameraControllerV2::topSpeed() const
{
    return m_flTopSpeed;
}

void CameraControllerV2::setTopSpeed(float speed)
{
    m_flTopSpeed = speed;
}

float CameraControllerV2::accelerationTime() const
{
    return m_flAccelTime;
}

void CameraControllerV2::setAccelerationTime(float acc)
{
    m_flAccelTime = acc;
}

float CameraControllerV2::decelerationTime() const
{
    return m_flDecelTime;
}

void CameraControllerV2::setDecelerationTime(float dec)
{
    m_flDecelTime = dec;
}

void CameraControllerV2::moveForward(bool pressed)
{
    if ( pressed ) m_iCurrentState |= Forward;
    else m_iCurrentState &= ~Forward;
}

void CameraControllerV2::moveBackward(bool pressed)
{
    if ( pressed ) m_iCurrentState |= Backward;
    else m_iCurrentState &= ~Backward;
}

void CameraControllerV2::moveRight(bool pressed)
{
    if ( pressed ) m_iCurrentState |= Right;
    else m_iCurrentState &= ~Right;
}

void CameraControllerV2::moveLeft(bool pressed)
{
    if ( pressed ) m_iCurrentState |= Left;
    else m_iCurrentState &= ~Left;
}

void CameraControllerV2::moveUp(bool pressed)
{
    if ( pressed ) m_iCurrentState |= Up;
    else m_iCurrentState &= ~Up;
}

void CameraControllerV2::moveDown(bool pressed)
{
    if ( pressed ) m_iCurrentState |= Down;
    else m_iCurrentState &= ~Down;
}

void CameraControllerV2::clearAllButtons()
{
    m_iCurrentState = None;
}

QVector3D CameraControllerV2::velocity() const
{
    return m_vecVelocity;
}

QVector3D CameraControllerV2::vectorFromButtons(int buttons)
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    
    if ( ((buttons & Right) == Right) && ((buttons & Left) != Left) ) x = 1.0f;
    else if ( ((buttons & Left) == Left) && ((buttons & Right) != Right) ) x = -1.0f;
    
    if ( ((buttons & Up) == Up) && ((buttons & Down) != Down) ) y = 1.0f;
    else if ( ((buttons & Down) == Down) && ((buttons & Up) != Up) ) y = -1.0f;
    
    if ( ((buttons & Forward) == Forward) && ((buttons & Backward) != Backward) ) z = 1.0f;
    else if ( ((buttons & Backward) == Backward) && ((buttons & Forward) != Forward) ) z = -1.0;
    
    return QVector3D(x, y, z);
}

// TODO: Merge these into one function - duplication is bad!
void CameraControllerV2::rotateViewTo(const QAngle &angle)
{
    if ( !m_pCamera ) return;
    
    // Get a vector from the current viewing angles.
    QVector3D viewVector = angle.toCameraVector();
    Q_ASSERT(qFuzzyCompare(viewVector.length(), 1.0f));
    
    // Get the distance of the eye from the view target.
    float viewDist = distanceEyeToCenter();
    
    // Calculate the new centre position and move the centre there.
    m_pCamera->setCenter(m_pCamera->eye() + (viewVector * viewDist));
    
    // Roll the camera - QAngle::toVector() does not provide any roll.
    m_pCamera->rotateEye(m_pCamera->roll(angle.roll()));
}

void CameraControllerV2::rotateCentreTo(const QAngle &angle)
{
    if ( !m_pCamera ) return;
    
    // Get a vector from the current viewing angles.
    // The parameter should be in the opposite direction to the player's view since we are
    // rotating the centre.
    QVector3D viewVector = angle.toCameraVector();
    Q_ASSERT(qFuzzyCompare(viewVector.length(), 1.0f));
    
    // Get the distance of the eye from the view target.
    float viewDist = distanceEyeToCenter();
    
    // Calculate the new eye position and move the camera there.
    m_pCamera->setEye(m_pCamera->center() + (viewVector * viewDist));
    
    // Roll the centre - QAngle::toVector() does not provide any roll.
    m_pCamera->rotateCenter(m_pCamera->roll(angle.roll()));
}

QAngle CameraControllerV2::viewAngles() const
{
    return m_qViewAngles;
}

void CameraControllerV2::setViewAngles(const QAngle &angle)
{
    if ( !enabled() ) return;
    
    m_qViewAngles = angle;
    rotateViewTo(m_qViewAngles);
}

void CameraControllerV2::setViewAnglesSilent(const QAngle &angle)
{
    m_qViewAngles = angle;
}

QAngle CameraControllerV2::centreAngles() const
{
    return m_qViewAngles.inverse();
}

void CameraControllerV2::setCentreAngles(const QAngle &angle)
{
    if ( !enabled() ) return;
    
    // For now, just adjust the viewpoint position when we orbit, ignoring any implications
    // this might have for the current travel speed and direction.
    m_qViewAngles = angle.inverse();
    rotateCentreTo(m_qViewAngles);
}

void CameraControllerV2::setCentreAnglesSilent(const QAngle &angle)
{
    m_qViewAngles = angle.inverse();
}

void CameraControllerV2::rotatePitch(float degrees)
{
    if ( !enabled() ) return;
    
    m_qViewAngles.setPitch(clampPitch(m_qViewAngles.pitch() + degrees, degrees < 0));
    rotateViewTo(m_qViewAngles);
}

void CameraControllerV2::rotateYaw(float degrees)
{
    if ( !enabled() ) return;
    
    m_qViewAngles.addYaw(degrees);
    rotateViewTo(m_qViewAngles);
}

void CameraControllerV2::rotateRoll(float degrees)
{
    if ( !enabled() ) return;
    
    m_qViewAngles.addRoll(degrees);
    rotateViewTo(m_qViewAngles);
}

void CameraControllerV2::orbitPitch(float degrees)
{
    if ( !enabled() ) return;
    
    m_qViewAngles.setPitch(clampPitch(m_qViewAngles.pitch() + degrees, degrees < 0));
    rotateCentreTo(m_qViewAngles.inverse());
}

void CameraControllerV2::orbitYaw(float degrees)
{
    if ( !enabled() ) return;
    
    m_qViewAngles.addYaw(degrees);
    rotateCentreTo(m_qViewAngles.inverse());
}

void CameraControllerV2::orbitRoll(float degrees)
{
    if ( !enabled() ) return;
    
    m_qViewAngles.addRoll(degrees);
    rotateCentreTo(m_qViewAngles.inverse());
}

// Returns the pitch value clamped to the correct range.
// If rotatedUp is true then pitch is clamped to the max upward angle, and vice versa.
float CameraControllerV2::clampPitch(float degrees, bool rotatedUp)
{
    // If the pitch value is outside the acceptable range:
    if ( degrees > PITCH_MAX_DELTA && degrees < 360.0f - PITCH_MAX_DELTA )
    {
        // Clamp it.
        return rotatedUp ? 360.0f - PITCH_MAX_DELTA : PITCH_MAX_DELTA;
    }
    
    
    // Otherwise, no change.
    return degrees;
}

bool CameraControllerV2::enabled() const
{
    return m_bEnabled;
}

void CameraControllerV2::setEnabled(bool b)
{
    m_bEnabled = b;
}

void CameraControllerV2::cameraChanged()
{
    if ( !m_pCamera ) return;
    
    // Update our angles from the camera.
    setViewAnglesSilent(QAngle(QGLCamera2::fromCameraSpace(m_pCamera->center() - m_pCamera->eye())));
}
