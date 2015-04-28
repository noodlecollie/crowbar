#include "qangle.h"
#include <QtMath>
#include <QVector3D>
#include "qglcamera2.h"

bool QAngle::operator ==(const QAngle &other)
{
    return pitch() == other.pitch() && yaw() == other.yaw() && roll() == other.roll();
}

bool QAngle::operator !=(const QAngle &other)
{
    return !(*this == other);
}

QAngle::QAngle()
{
    m_flPitch = m_flYaw = m_flRoll = 0.0f;
}

QAngle::QAngle(float pitch, float yaw, float roll)
{
    setPitch(pitch);
    setYaw(yaw);
    setRoll(roll);
}

QAngle::QAngle(const QVector3D &direction)
{
    *this = vectorToAngle(direction);
}

QAngle::QAngle(const QAngle &other)
{
    *this = other;
}

QAngle& QAngle::operator =(const QAngle &other)
{
    setPitch(other.pitch());
    setYaw(other.yaw());
    setRoll(other.roll());
    
    return *this;
}

QAngle QAngle::operator +(const QAngle &other)
{
    return QAngle(pitch() + other.pitch(), yaw() + other.yaw(), roll() + other.roll());
}

QAngle QAngle::operator -(const QAngle &other)
{
    return QAngle(pitch() - other.pitch(), yaw() - other.yaw(), roll() - other.roll());
}

QAngle& QAngle::operator+=(const QAngle &other)
{
    setPitch(pitch() + other.pitch());
    setYaw(yaw() + other.yaw());
    setRoll(roll() + other.roll());
    
    return *this;
}

QAngle& QAngle::operator-=(const QAngle &other)
{
    setPitch(pitch() - other.pitch());
    setYaw(yaw() - other.yaw());
    setRoll(roll() - other.roll());
    
    return *this;
}

QAngle QAngle::vectorToAngle(const QVector3D &vector)
{
    // Default angle of 0,0,0 looks down the positive x-axis.

    if (vector.y() == 0.0f && vector.x() == 0.0f)
    {
        // If the vector is null, return a zeroed angle.
        if ( vector.z() == 0.0f ) return QAngle();
        
        return vector.z() > 0.0f ? QAngle(270,0,0) : QAngle(90,0,0);
    }
    
    float tmp, yaw, pitch;
    
    yaw = (qAtan2(vector.y(), vector.x()) * 180 / M_PI);
    if ( yaw < 0.0f ) yaw += 360;

    tmp = qSqrt(vector.x()*vector.x() + vector.y()*vector.y());
    pitch = (qAtan2(-vector.z(), tmp) * 180 / M_PI);
    if ( pitch < 0.0f ) pitch += 360;
    
    return QAngle(pitch, yaw, 0);
}

QVector3D QAngle::angleToVector(const QAngle &angle)
{
    // In HL2 code, SinCos was an assembly function that returned the sin and the cos of a given angle.
    // Unless we need this raw speed (maybe we might in the future?), just use normal, readable sin/cos functions.
    
    float radYaw = qDegreesToRadians(angle.yaw());
    float radPitch = qDegreesToRadians(angle.pitch());
    float sinPitch = qSin(radPitch);
    float sinYaw = qSin(radYaw);
    float cosPitch = qCos(radPitch);
    float cosYaw = qCos(radYaw);
    
    return QVector3D(cosPitch * cosYaw, cosPitch * sinYaw, -sinPitch);
}

float QAngle::pitch() const
{
    return m_flPitch;
}

float QAngle::yaw() const
{
    return m_flYaw;
}

float QAngle::roll() const
{
    return m_flRoll;
}

void QAngle::setPitch(float f)
{
    m_flPitch = clampDegrees(f);
}

void QAngle::setYaw(float f)
{
    m_flYaw = clampDegrees(f);
}

void QAngle::setRoll(float f)
{
    m_flRoll = clampDegrees(f);
}

void QAngle::addPitch(float f)
{
    m_flPitch = clampDegrees(m_flPitch  + f);
}

void QAngle::addYaw(float f)
{
    m_flYaw = clampDegrees(m_flYaw  + f);
}

void QAngle::addRoll(float f)
{
    m_flRoll = clampDegrees(m_flRoll  + f);
}

QVector3D QAngle::toCameraVector() const
{
    return QGLCamera2::toCameraSpace(toVector());
}

void QAngle::add(const QAngle &angle)
{
    addPitch(angle.pitch());
    addYaw(angle.yaw());
    addRoll(angle.roll());
}

void QAngle::subtract(const QAngle &angle)
{
    subtractPitch(angle.pitch());
    subtractYaw(angle.yaw());
    subtractRoll(angle.roll());
}

void QAngle::flipPitch()
{
    setPitch(pitchFlipped());
}

void QAngle::invert()
{
    *this = inverse();
}

QDebug& operator<<(QDebug &dbg, const QAngle &angle)
{
     dbg.nospace() << "QAngle(" << angle.pitch() << ", " << angle.yaw() << ", " << angle.roll() << ")";

     return dbg.space();
}
