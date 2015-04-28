#ifndef CAMERACONTROLLERV2_H
#define CAMERACONTROLLERV2_H

#include <QObject>
#include <QPointer>
#include <QGLCamera>
#include <QVector3D>
#include <QQuaternion>
#include "qangle.h"

// Maximum pitch deviation from 0 (horizontal).
#define PITCH_MAX_DELTA 89.0f

class CameraControllerV2 : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QGLCamera* camera READ camera WRITE setCamera)
    Q_PROPERTY(float topSpeed READ topSpeed WRITE setTopSpeed)
    Q_PROPERTY(float accelerationTime READ accelerationTime WRITE setAccelerationTime)
    Q_PROPERTY(float decelerationTime READ decelerationTime WRITE setDecelerationTime)
    Q_PROPERTY(QVector3D velocity READ velocity)
    Q_PROPERTY(QAngle viewAngles READ viewAngles WRITE setViewAngles)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled)
public:
    enum MovementFlags
    {
        None = 0x0,
        Forward = 0x1,
        Backward = 0x2,
        Right = 0x4,
        Left = 0x8,
        Up = 0x10,
        Down = 0x20
    };
    
    explicit CameraControllerV2(QObject *parent = 0);
    
    QGLCamera* camera() const;
    void setCamera(QGLCamera* camera);
    
    // Top speed in units per second.
    float topSpeed() const;
    void setTopSpeed(float speed);
    
    // Time, in seconds, that it takes to reach top speed.
    float accelerationTime() const;
    void setAccelerationTime(float acc);
    
    // Time, in seconds, that it takes to stop from top speed.
    float decelerationTime() const;
    void setDecelerationTime(float dec);
    
    QVector3D velocity() const;
    QAngle viewAngles() const;
    QAngle centreAngles() const;
    
    bool enabled() const;
    void setEnabled(bool b);
    
    inline float distanceEyeToCenter() const;
    
signals:
    
public slots:
    void processMovement(int dMsec);
    
    void moveForward(bool pressed);
    void moveBackward(bool pressed);
    void moveRight(bool pressed);
    void moveLeft(bool pressed);
    void moveUp(bool pressed);
    void moveDown(bool pressed);
    
    void rotatePitch(float degrees);
    void rotateYaw(float degrees);
    void rotateRoll(float degrees);
    
    void orbitPitch(float degrees);
    void orbitYaw(float degrees);
    void orbitRoll(float degrees);
    
    void clearAllButtons();
    
    void setViewAngles(const QAngle &angle);
    void setCentreAngles(const QAngle &angle);
    
    // Does not update the camera once the angles have been set.
    // Used if the controller is disabled but the angles still need to be updated.
    void setViewAnglesSilent(const QAngle &angle);
    void setCentreAnglesSilent(const QAngle &angle);
    
private slots:
    void cameraChanged();
    
private:
    static QVector3D vectorFromButtons(int buttons);
    static QQuaternion yawRotation(float degrees);
    static float clampPitch(float degrees, bool rotatedUp);
    
    void init();
    void calculateVelocityVector(int msec);
    void applyCameraTranslation(int msec);
    void rotateViewTo(const QAngle &angle);
    void rotateCentreTo(const QAngle &angle);
    
    QPointer<QGLCamera> m_pCamera;
    float               m_flTopSpeed;
    float               m_flAccelTime;
    float               m_flDecelTime;
    int                 m_iCurrentState;
    QVector3D           m_vecVelocity;      // Local to camera, in units per second.
    QAngle              m_qViewAngles;
    bool                m_bEnabled;
};

inline float CameraControllerV2::distanceEyeToCenter() const
{
    return (m_pCamera->eye() - m_pCamera->center()).length();
}

#endif // CAMERACONTROLLERV2_H
