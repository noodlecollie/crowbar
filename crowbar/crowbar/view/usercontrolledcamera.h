#ifndef USERCONTROLLEDCAMERA_H
#define USERCONTROLLEDCAMERA_H

#include "view_global.h"
#include "glanglecamera.h"

VIEW_BEGIN_NAMESPACE

class VIEWSHARED_EXPORT UserControlledCamera : public GLAngleCamera
{
    Q_OBJECT
    Q_ENUMS(MovementFlags)
public:
    // - Forward/backward are always along the view direction of the camera.
    // - Left/right are along the world XY plane in the direction of the cross
    //      product of the view vector and (0,0,1).
    // - Up/down are always along the world Z axis.
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

    explicit UserControlledCamera(QObject *parent = 0);

    float topSpeed() const;
    void setTopSpeed(float speed);
    float accelTime() const;
    void setAccelTime(float seconds);
    float decelTime() const;
    void setDecelTime(float seconds);

    void forward(bool pressed);
    void backward(bool pressed);
    void left(bool pressed);
    void right(bool pressed);
    void up(bool pressed);
    void down(bool pressed);
    void clearMovementFlags();

    // setVelocity() clamps the length of the velocity vector to topSpeed().
    // x: +right    -left
    // y: +forwards -backwards
    // z: +up       -down
    QVector3D velocity() const;
    void setVelocity(const QVector3D &velocity);

signals:

public slots:
    void updateFromVelocity();

private:
    typedef int MovementState;

    void init();
    static inline QVector3D clampLength(const QVector3D &vec, float length);
    static QVector3D vectorFromButtons(MovementState buttons);
    void translateCameraLocal(const QVector3D translation);
    void setVelocityNoClamp(const QVector3D &velocity);

    QVector3D       m_vecVelocity;          // Current velocity in units per second, local to the camera.
    MovementState   m_iMovementFlags;       // Current movement flags.
    float           m_flTopSpeed;           // Top speed in units per second.
    float           m_flAccelTime;          // Time taken to accelerate from zero to top speed, in seconds.
    float           m_flDecelTime;          // Time taken to decelerate from top speed to zero, in seconds.
};

inline QVector3D UserControlledCamera::clampLength(const QVector3D &vec, float length)
{
    return vec.lengthSquared() > length * length ? vec.normalized() * length : vec;
}

VIEW_END_NAMESPACE

#endif // USERCONTROLLEDCAMERA_H
