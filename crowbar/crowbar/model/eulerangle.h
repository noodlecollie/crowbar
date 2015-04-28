#ifndef EULERANGLE_H
#define EULERANGLE_H

#include "model_global.h"
#include <QVector3D>
#include <QMatrix4x4>

class QVector3D;
class QDebug;
class QMatrix4x4;

MODEL_BEGIN_NAMESPACE

/**
 * @brief The EulerAngle class defines an angle using pitch, yaw and roll.
 * 
 * The following specification is written to imitate how Euler angles work in Hammer.
 * The left-handed Hammer co-ordinate system is used: X points right, Z points up, Y points away.
 * 
 * (0, 0, 0) points down the positive X axis, with the up vector pointing up the positive vertical Z axis.
 * (-90, 0, 0) points straight up the positive Z axis, with the up vector pointing down the negative X axis.
 * (90, 0, 0) points straight down the negative Z axis, with the up vector pointing up the positive X axis.
 * (0, 90, 0) points up the positive Y axis, with the up vector still pointing up the positive vertical Z axis.
 * (0, 0, 90) points up the positive X axis with the up vector pointing down the negative Y axis.
 * (0, 0, -90) points up the positive X axis with the up vector pointing up the positive Y axis.
 * 
 * Therefore the pitch, yaw and roll can be thought of in the following way:
 * 
 * Yaw should always be applied around the world Z axis. A yaw of 0 points down the world X axis; increasing the yaw
 * rotates anticlockwise around the world Z axis.
 * 
 * Pitch is applied after yaw. Negative pitch values rotate upwards to point up the Z axis; positive values rotate downwards.
 * 
 * Roll is applied after pitch. Positive roll values rotate clockwise around the vector specified by pitch and yaw;
 * negative values rotate anticlockwise.
 */
class MODELSHARED_EXPORT EulerAngle
{
public:
    /**
     * @brief Constructs an EulerAngle with the specified \a pitch, \a yaw and \a roll.
     * @param pitch Pitch value, in degrees.
     * @param yaw Yaw value, in degrees.
     * @param roll Roll value, in degrees.
     */
    EulerAngle(float pitch, float yaw, float roll);
    
    /**
     * @brief Constructs an EulerAngke with pitch, yaw and roll set to 0.
     */
    EulerAngle();
    
    /**
     * @brief Returns the pitch, in degrees.
     * @return Pitch in degrees.
     */
    float pitch() const;
    
    /**
     * @brief Returns the yaw in degrees.
     * @return Yaw in degrees.
     */
    float yaw() const;
    
    /**
     * @brief Returns the roll in degrees.
     * @return Roll in degrees.
     */
    float roll() const;
    
    /**
     * @brief Sets the pitch in degrees.
     * @param value Pitch value to set.
     */
    void setPitch(float value);
    
    /**
     * @brief Sets the yaw in degrees.
     * @param value Yaw value to set.
     */
    void setYaw(float value);
    
    /**
     * @brief Sets the roll in degrees.
     * @param value Roll value to set.
     */
    void setRoll(float value);
    
    /**
     * @brief Compares for equality using an exact floating point comparison.
     * @param other Angle to compare against.
     * @return True if pitch, yaw and roll on this angle are exactly the same as on the other angle;
     * false otherwise.
     */
    bool operator ==(const EulerAngle &other) const;
    
    /**
     * @brief Compares for inequality using an exact floating point comparison.
     * @param other Angle to compare against.
     * @return False if pitch, yaw and roll on this angle are exactly the same as on the other angle;
     * true otherwise.
     */
    bool operator !=(const EulerAngle &other) const;
    
    /**
     * @brief Returns a new angle whose pitch, yaw and roll are the sum of this and the other
     * angle's pitch, yaw and roll respectively.
     * @param other Angle to add.
     * @return New angle containing the sum of this and the other angle.
     */
    EulerAngle operator +(const EulerAngle &other) const;
    
    /**
     * @brief Returns a new angle whose pitch, yaw and roll are the difference of this and the other
     * angle's pitch, yaw and roll respectively.
     * @param other Angle to subtract.
     * @return New angle containing the difference of this and the other angle.
     */
    EulerAngle operator -(const EulerAngle &other) const;
    
    /**
     * @brief Adds the pitch, yaw and roll of the other angle to this angle
     * and returns a reference to this angle.
     * @param other Angle to add.
     * @return Reference to this angle.
     */
    EulerAngle& operator +=(const EulerAngle &other);
    
    /**
     * @brief Subtracts the pitch, yaw and roll of the other angle from this angle
     * and returns a reference to this angle.
     * @param other Angle to subtract.
     * @return Reference to this angle.
     */
    EulerAngle& operator -=(const EulerAngle &other);
    
    /**
     * @brief Returns an EulerAngle with the pitch and yaw set by the direction of the input vector,
     * and with the roll set to 0. Pitch and yaw should be normalised within the range [0 360).
     * If the vector is null, pitch, yaw and roll should all be set to 0.
     * @param vec Vector to convert.
     * @return EulerAngle representing the vector.
     */
    static EulerAngle vectorToAngle(const QVector3D &vec);
    
    /**
     * @brief Returns a normalised vector representing the pitch and yaw components of the
     * given angle, corresponsing to the specification written at the beginning of this class.
     * @param angle Angle to convert.
     * @return Vector representing the angle.
     */
    static QVector3D angleToVector(const EulerAngle &angle);
    inline QVector3D toVector() const { return angleToVector(*this); }
    
    /**
     * @brief Normalises this angle's pitch, yaw and roll to be within the range [0 360] and returns
     * a reference to this angle.
     * @return Reference to this angle.
     */
    EulerAngle& normalise();
    
    /**
     * @brief Returns a new angle which is a normalised copy of this angle.
     * @return Normalised copy.
     */
    EulerAngle normalised() const;
    
    /**
     * @brief Negates the pitch value and adds 180 degrees to the yaw value of this angle,
     * and returns a reference to this angle. Roll is unaffected.
     * @return Reference to this angle.
     */
    EulerAngle& makeOpposite();
    
    /**
     * @brief Returns a new angle which is a copy of this angle, with the pitch negated and with
     * the yaw increased by 180 degrees. Roll remains unaffected.
     * @return Opposite copy.
     */
    EulerAngle opposite() const;

    /**
     * @brief Returns the rotation matrix corresponding to this angle.
     * The rotation is with respect to the (0,0,0) angle.
     * @return Matrix representing this angle's rotation.
     */
    QMatrix4x4 rotationMatrix() const;
    
private:
    float   m_flPitch;  /// Pitch value.
    float   m_flYaw;    /// Yaw value.
    float   m_flRoll;   /// Roll value.
    
    /**
     * @brief Returns a value for the given angle, normalised within [0 360).
     * @param degrees Angle to normalise.
     * @return Normalised version of this angle.
     */
    static float normalised(float degrees);
};

QDebug MODELSHARED_EXPORT operator<<(QDebug dbg, const EulerAngle &angle);

MODEL_END_NAMESPACE

#endif // EULERANGLE_H
