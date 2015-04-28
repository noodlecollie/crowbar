#include "glanglecamera.h"
#include <QQuaternion>
#include <QtMath>
#include <QtDebug>

using namespace MODEL_NAMESPACE;

VIEW_BEGIN_NAMESPACE

GLAngleCamera::GLAngleCamera(QObject *parent) :
    QGLCamera(parent), m_flMaxPitchDelta(MAX_PITCH_DELTA), m_Angles(defaultViewAngles()),
    m_flEyeCentreDistance(1.0f)
{
    resetPositionAndAngles();
}

QVector3D GLAngleCamera::defaultCenter()
{
    return QVector3D(0,1,0);
}

QVector3D GLAngleCamera::defaultEye()
{
    return QVector3D(0,0,0);
}

QVector3D GLAngleCamera::defaultUpVector()
{
    return QVector3D(0,0,1);
}

EulerAngle GLAngleCamera::defaultViewAngles()
{
    return EulerAngle(0,0,0);
}

QVector3D GLAngleCamera::defaultViewVector()
{
    return QVector3D(0,1,0);
}

void GLAngleCamera::resetPositionAndAngles()
{
    transform(defaultViewAngles(), defaultEye(), 1.0f);
}

void GLAngleCamera::transform(const EulerAngle &viewAngles, const QVector3D &eyePosition, float eyeCentreDistance)
{
    // Begin with the eye at the origin and the centre one unit down the Y axis.
    QVector3D vEye(0,0,0);
    QVector3D vCentre(0,1,0);
    QVector3D viewDir(0,1,0);
    QVector3D upDir(0,0,1);

    // Rotate the view direction by the yaw of the angle.
    QQuaternion qYaw = QQuaternion::fromAxisAndAngle(upDir, viewAngles.yaw());
    viewDir = qYaw.rotatedVector(viewDir);

    // Rotate the view direction by the pitch of the angle.
    // We want to do this around the new right vector, which can be found by
    // doing the cross product of the view vector and the up vector (still (0,0,1)).
    QVector3D right = QVector3D::crossProduct(viewDir, QVector3D(0,0,1));
    float pitch = viewAngles.pitch();
    if ( pitch > maxPitchDelta() ) pitch = maxPitchDelta();
    else if ( pitch < -maxPitchDelta() ) pitch = -maxPitchDelta();
    QQuaternion qPitch = QQuaternion::fromAxisAndAngle(right, -pitch);
    viewDir = qPitch.rotatedVector(viewDir);
    upDir = qPitch.rotatedVector(upDir);

    // Rotate the up vector around the view vector.
    float roll = viewAngles.roll();

    if ( roll > 180.0f ) roll = 180.0f;
    else if ( roll < -180.0f ) roll = -180.0f;
    QQuaternion qRoll = QQuaternion::fromAxisAndAngle(viewDir, roll);
    upDir = qRoll.rotatedVector(upDir);

    Q_ASSERT(qFuzzyCompare(upDir.length(), 1.0f));
    Q_ASSERT(qFuzzyCompare(viewDir.length(), 1.0f));

    // Set the centre position according to the new view vector.
    vCentre = vEye + (eyeCentreDistance * viewDir);

    // Set the eye and centre positions in space.
    vEye += eyePosition;
    vCentre += eyePosition;

    // Set all the relevant bits and pieces on the camera.
    setEye(vEye);
    setCenter(vCentre);
    setUpVector(upDir);
}

float GLAngleCamera::maxPitchDelta() const
{
    return m_flMaxPitchDelta;
}

void GLAngleCamera::setMaxPitchDelta(float delta)
{
    if ( delta > MAX_PITCH_DELTA ) delta = MAX_PITCH_DELTA;
    else if ( delta < -MAX_PITCH_DELTA ) delta = -MAX_PITCH_DELTA;
    delta = qAbs(delta);

    if ( delta == m_flMaxPitchDelta ) return;

    m_flMaxPitchDelta = delta;
    emit maxPitchDeltaChanged(m_flMaxPitchDelta);
}

EulerAngle GLAngleCamera::viewAngles() const
{
    return m_Angles;
}

void GLAngleCamera::setViewAngles(const EulerAngle &angles)
{
    if ( angles == m_Angles ) return;

    m_Angles = angles;

    // We want to normalise yaw only - pitch and roll are specified using +/- values.
    // Neither pitch nor roll are clamped here, only in addPitch()/addRoll() - be careful!
    while ( m_Angles.yaw() >= 360.0f ) m_Angles.setYaw(m_Angles.yaw() - 360.0f);
    while ( m_Angles.yaw() < 0.0f ) m_Angles.setYaw(m_Angles.yaw() + 360.0f);

    transform(m_Angles, eye(), eyeCenterDistance());
}

void GLAngleCamera::addPitch(float pitch)
{
    EulerAngle angle = viewAngles();

    if ( angle.pitch() + pitch > maxPitchDelta() )
    {
        pitch = maxPitchDelta() - angle.pitch();
    }
    else if ( angle.pitch() + pitch < -maxPitchDelta() )
    {
        pitch = -(maxPitchDelta() + angle.pitch());
    }

    angle.setPitch(angle.pitch() + pitch);
    setViewAngles(angle);
}

void GLAngleCamera::addYaw(float yaw)
{
    EulerAngle angle = viewAngles();
    angle.setYaw(angle.yaw() + yaw);
    setViewAngles(angle);
}

void GLAngleCamera::addRoll(float roll)
{
    EulerAngle angle = viewAngles();

    if ( angle.roll() + roll > 180.0f )
    {
        roll = 180.0f - angle.roll();
    }
    else if ( angle.roll() + roll < -180.0f )
    {
        roll = -(180.0f + angle.roll());
    }

    angle.setRoll(angle.roll() + roll);
    setViewAngles(angle);
}

float GLAngleCamera::eyeCenterDistance() const
{
    return m_flEyeCentreDistance;
}

void GLAngleCamera::setEyeCenterDistance(float distance)
{
    if ( distance == m_flEyeCentreDistance ) return;

    m_flEyeCentreDistance = distance;
    applyEyeCentreDistance();
    emit eyeCenterDistanceChanged(m_flEyeCentreDistance);
}

QVector3D GLAngleCamera::viewVector() const
{
    return (center() - eye()).normalized();
}

void GLAngleCamera::applyEyeCentreDistance()
{
    setCenter(eye() + (eyeCenterDistance() * viewVector()));
}

void GLAngleCamera::setViewVector(const QVector3D &dir)
{
    QVector3D nDir = dir.normalized();
    setViewAngles(EulerAngle::vectorToAngle(nDir));
}

QVector3D GLAngleCamera::rightVector() const
{
    return QVector3D::crossProduct(viewVector(), upVector());
}

float GLAngleCamera::zoom() const
{
    return 1.0f / viewSize().height();
}

void GLAngleCamera::setZoom(float zoom)
{
    setViewSize(QSizeF(1.0f/zoom, 1.0f/zoom));
}

void GLAngleCamera::setEyeCenterMaintainOffset(const QVector3D &pos)
{
    QVector3D viewDir = viewVector();
    float dist = eyeCenterDistance();
    setEye(pos);
    setCenter(pos + (viewDir * dist));
}

void GLAngleCamera::updateViewAnglesForCurrentPosition()
{
    QVector3D lookVector = center() - eye();
    float dist = lookVector.length();
    setEyeCenterDistance(dist);

    QVector3D vDir = lookVector.normalized();
    EulerAngle vAng = normaliseAngle(EulerAngle::vectorToAngle(vDir));
    setViewAngles(vAng);
}

EulerAngle GLAngleCamera::normaliseAngle(const EulerAngle &angle)
{
    EulerAngle a(angle);

    if ( a.pitch() > maxPitchDelta() )
    {
        a.setPitch(maxPitchDelta());
    }
    else if ( a.pitch() < -maxPitchDelta() )
    {
        a.setPitch(-maxPitchDelta());
    }

    if ( a.roll() > 180.0f )
    {
        a.setRoll(180.0f);
    }
    else if ( a.roll() < -180.0f )
    {
        a.setRoll(-180.0f);
    }

    while ( a.yaw() < 0.0f ) a.setYaw(a.yaw() + 360.0f);
    while ( a.yaw() >= 360.0f ) a.setYaw(a.yaw() - 360.0f);

    return a;
}

VIEW_END_NAMESPACE
