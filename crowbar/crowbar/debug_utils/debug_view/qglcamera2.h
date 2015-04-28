#ifndef QGLCAMERA2_H
#define QGLCAMERA2_H

#include <QGLCamera>
#include "qangle.h"

#define ENSURE_VIEW_CLAMP
#define DEFAULT_E_C_DIST    10.0f

class QGLCamera2 : public QGLCamera
{
    Q_OBJECT
    Q_ENUMS(MovementFlags)
    Q_PROPERTY(QVector3D velocity READ velocity WRITE setVelocity)
    Q_PROPERTY(float topSpeed READ topSpeed WRITE setTopSpeed)
    Q_PROPERTY(float accelTime READ accelTime WRITE setAccelTime)
    Q_PROPERTY(float decelTime READ decelTime WRITE setDecelTime)
    Q_PROPERTY(QVector3D viewVector READ viewVector WRITE setViewVector)
    Q_PROPERTY(QAngle viewAngles READ viewAngles WRITE setViewAngles)
    Q_PROPERTY(float maxPitchDelta READ maxPitchDelta WRITE setMaxPitchDelta)
    Q_PROPERTY(float defaultEyeCenterDistance READ defaultEyeCenterDistance WRITE setDefaultEyeCenterDistance RESET resetDefaultEyeCenterDistance)
public:
    explicit QGLCamera2(QObject *parent = 0);
    
    // Movement is relative to camera apart from up/down, which are global.
    enum MovementFlags
    {
        NoMovement = 0x0,
        MoveForward = 0x1,
        MoveBackward = 0x2,
        MoveRight = 0x4,
        MoveLeft = 0x8,
        MoveUp = 0x10,
        MoveDown = 0x20
    };
    
    inline float distanceFromEyeToCenter() const;   // Distance in units from the camera's eye to its centre.
    
    // topSpeed
    float topSpeed() const;
    void setTopSpeed(float speed);
    
    // accelTime
    float accelTime() const;
    void setAccelTime(float seconds);
    
    // decelTime
    float decelTime() const;
    void setDecelTime(float seconds);
    
    // velocity
    // setVelocity() clamps the length of the velocity vector to topSpeed().
    // x: +right    -left
    // y: +up       -down
    // z: +forwards -backwards
    QVector3D velocity() const;
    void setVelocity(const QVector3D &velocity);
    
    // viewVector
    inline QVector3D viewVector() const;
    inline void setViewVector(const QVector3D &vec);
    inline void setViewVectorDefaultDistance(const QVector3D &vec); // Uses default eye-centre distance.
    
    // viewAngles
    QAngle viewAngles() const;
    void setViewAngles(const QAngle &angles);
    
    // maxPitchDelta
    float maxPitchDelta() const;
    void setMaxPitchDelta(float delta);
    
    // defaultEyeCenterDistance
    float defaultEyeCenterDistance() const;
    void setDefaultEyeCenterDistance(float f);
    void resetDefaultEyeCenterDistance();
    
    static inline QVector3D toCameraSpace(const QVector3D &vec);
    static inline QVector3D fromCameraSpace(const QVector3D &vec);
    
#ifndef ENSURE_VIEW_CLAMP
protected slots:
    // Making these non-public because they don't clamp the view angles.
    void tiltPanRollCenter(float tiltAngle, float panAngle, float rollAngle, RotateOrder order);
    void tiltPanRollEye(float tiltAngle, float panAngle, float rollAngle, RotateOrder order);
#endif  // ENDURE_VIEW_CLAMP
    
public:
signals:
    
public slots:
    void updateFromVelocity();      // Updates the position of the camera according to the current velocity.
    inline void translateCamera(const QVector3D translation);
    inline void translateEye(const QVector3D translation);
    inline void translateCenter(const QVector3D translation);
    
    void forward(bool pressed);
    void backward(bool pressed);
    void left(bool pressed);
    void right(bool pressed);
    void up(bool pressed);
    void down(bool pressed);
    void clearMovementFlags();
    
    void addPitch(float f);
    void addYaw(float f);
    void addRoll(float f);
    inline void subtractPitch(float f) {addPitch(-f);}
    inline void subtractYaw(float f) {addYaw(-f);}
    inline void subtractRoll(float f) {addRoll(-f);}
    
private:
    typedef int MovementState;
    
    void init();
    void setVelocityNoClamp(const QVector3D &velocity);
    void simpleViewAngleClamp();
    void viewAngleClamp(bool rotateUpwards);
    static inline QVector3D clampLength(const QVector3D &vec, float length);
    static QVector3D vectorFromButtons(MovementState buttons);
    
    QVector3D       m_vecVelocity;          // Current velocity in units per second, local to the camera.
    MovementState   m_iMovementFlags;       // Current movement flags.
    float           m_flTopSpeed;           // Top speed in units per second.
    float           m_flAccelTime;          // Time taken to accelerate from zero to top speed, in seconds.
    float           m_flDecelTime;          // Time taken to decelerate from top speed to zero, in seconds.
    float           m_flMaxPitchDelta;      // Maximum angle the camera is allowed to tilt up or down.
    float           m_flDefEyeCenterDist;   // Default distance between the centre and the eye.
};

inline float QGLCamera2::distanceFromEyeToCenter() const
{
    return (center() - eye()).length();
}

inline QVector3D QGLCamera2::viewVector() const
{
    return (center() - eye()).normalized();
}

inline void QGLCamera2::setViewVector(const QVector3D &vec)
{
    setCenter(eye() + (vec.normalized() * distanceFromEyeToCenter()));
}

inline void QGLCamera2::setViewVectorDefaultDistance(const QVector3D &vec)
{
    setCenter(eye() + (vec.normalized() * defaultEyeCenterDistance()));
}

inline QVector3D QGLCamera2::clampLength(const QVector3D &vec, float length)
{
    return vec.lengthSquared() > length * length ? vec.normalized() * length : vec;
}

inline void QGLCamera2::translateEye(const QVector3D translation)
{
    QGLCamera::translateEye(translation.x(), translation.y(), translation.z());
}

inline void QGLCamera2::translateCenter(const QVector3D translation)
{
    QGLCamera::translateCenter(translation.x(), translation.y(), translation.z());
}

inline void QGLCamera2::translateCamera(const QVector3D translation)
{
    translateCenter(translation);
    translateEye(translation);
}

// Turns out these two are identical as they just swap y and z.
inline QVector3D QGLCamera2::toCameraSpace(const QVector3D &vec)
{
    return QVector3D(vec.x(), vec.z(), vec.y());
}

inline QVector3D QGLCamera2::fromCameraSpace(const QVector3D &vec)
{
    return QVector3D(vec.x(), vec.z(), vec.y());
}

#endif // QGLCAMERA2_H
