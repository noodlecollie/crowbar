#include "eulerangle.h"
#include <QtMath>
#include <QtDebug>
#include "model_util.h"

MODEL_BEGIN_NAMESPACE

EulerAngle::EulerAngle(float pitch, float yaw, float roll)
{
    m_flPitch = pitch;
    m_flYaw = yaw;
    m_flRoll = roll;
}

EulerAngle::EulerAngle()
{
    m_flPitch = m_flYaw = m_flRoll = 0.0f;
}

float EulerAngle::pitch() const
{
    return m_flPitch;
}

float EulerAngle::yaw() const
{
    return m_flYaw;
}

float EulerAngle::roll() const
{
    return m_flRoll;
}

void EulerAngle::setPitch(float value)
{
    m_flPitch = value;
}

void EulerAngle::setYaw(float value)
{
    m_flYaw = value;
}

void EulerAngle::setRoll(float value)
{
    m_flRoll = value;
}

bool EulerAngle::operator ==(const EulerAngle &other) const
{
    return ((qFuzzyIsNull(pitch()) && qFuzzyIsNull(other.pitch())) ||
            qFuzzyCompare(pitch(), other.pitch())) &&
            
           ((qFuzzyIsNull(yaw()) && qFuzzyIsNull(other.yaw())) ||
            qFuzzyCompare(yaw(), other.yaw())) &&
            
           ((qFuzzyIsNull(roll()) && qFuzzyIsNull(other.roll())) ||
            qFuzzyCompare(roll(), other.roll()));
}

bool EulerAngle::operator !=(const EulerAngle &other) const
{
    return !(*this == other);
}

EulerAngle EulerAngle::operator +(const EulerAngle &other) const
{
    return EulerAngle(pitch() + other.pitch(),
                      yaw() + other.yaw(),
                      roll() + other.roll());
}

EulerAngle EulerAngle::operator -(const EulerAngle &other) const
{
    return EulerAngle(pitch() - other.pitch(),
                      yaw() - other.yaw(),
                      roll() - other.roll());
}

EulerAngle& EulerAngle::operator +=(const EulerAngle &other)
{
    setPitch(pitch() + other.pitch());
    setYaw(yaw() + other.yaw());
    setRoll(roll() + other.roll());
    return *this;
}

EulerAngle& EulerAngle::operator -=(const EulerAngle &other)
{
    setPitch(pitch() - other.pitch());
    setYaw(yaw() - other.yaw());
    setRoll(roll() - other.roll());
    return *this;
}

// \cite http://www.gamedev.net/topic/399701-convert-vector-to-euler-cardan-angles/#entry3651854
EulerAngle EulerAngle::vectorToAngle(const QVector3D &vec)
{
    // If the vector is null, return a zeroed angle.
    if ( vec.isNull() )
    {
        return EulerAngle();
    }
    
    // If x and y are null, just set the pitch.
    if ( qFuzzyIsNull(vec.x()) && qFuzzyIsNull(vec.y()) )
    {
        return EulerAngle(vec.z() > 0.0f ? 270.0f : 90.0f, 0.0f, 0.0f);
    }
    
    float temp, yaw, pitch;
        
    // Yaw depends on the x and y co-ordinates.
    yaw = qRadiansToDegrees(qAtan2(vec.y(), vec.x()));
    if ( yaw < 0.0f ) yaw += 360.0f;

    // Pitch is found by finding the angle between the xy projection of the vector
    // and the negative Z axis.
    temp = qSqrt(vec.x()*vec.x() + vec.y()*vec.y());        // Length of projection onto xy plane
    pitch = qRadiansToDegrees(qAtan(-vec.z() / temp));      // Angle between this and -z.
    if ( pitch < 0.0f ) pitch += 360;
    
    return EulerAngle(pitch, yaw, 0.0f);
}

QVector3D EulerAngle::angleToVector(const EulerAngle &angle)
{
    // X and Z are dependent on the pitch and yaw; Y is dependent on the pitch only.
    float radYaw = qDegreesToRadians(angle.yaw());
    float radPitch = qDegreesToRadians(angle.pitch());
    float sinPitch = qSin(radPitch);
    float sinYaw = qSin(radYaw);
    float cosPitch = qCos(radPitch);
    float cosYaw = qCos(radYaw);
    
    return QVector3D(cosPitch * cosYaw, cosPitch * sinYaw, -sinPitch);
}

EulerAngle& EulerAngle::normalise()
{
    m_flPitch = normalised(m_flPitch);
    m_flYaw = normalised(m_flYaw);
    m_flRoll = normalised(m_flRoll);
    return *this;
}

float EulerAngle::normalised(float degrees)
{
    while (degrees < 0.0f) degrees += 360.0f;
    while (degrees >= 360.0f) degrees -= 360.0f;
    return degrees;
}

EulerAngle EulerAngle::normalised() const
{
    return EulerAngle(normalised(pitch()), normalised(yaw()), normalised(roll()));  
}

EulerAngle& EulerAngle::makeOpposite()
{
    m_flPitch = -m_flPitch;
    m_flYaw += 180.0f;
    return *this;
}

EulerAngle EulerAngle::opposite() const
{
    return EulerAngle(-pitch(), yaw() + 180.0f, roll());
}

QDebug operator<<(QDebug dbg, const EulerAngle &angle)
{
    dbg.nospace() << "EulerAngle(" << angle.pitch() << "," << angle.yaw() << "," << angle.roll() << ")";
    
    return dbg.space();
}

QMatrix4x4 EulerAngle::rotationMatrix() const
{
    return Model_Util::rotationMatrix(*this);
}

MODEL_END_NAMESPACE
