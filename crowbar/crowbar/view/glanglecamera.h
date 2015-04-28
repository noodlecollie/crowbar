#ifndef GLANGLECAMERA_H
#define GLANGLECAMERA_H

#include "view_global.h"
#include <QGLCamera>
#include "eulerangle.h"

#define MAX_PITCH_DELTA 89.0f

VIEW_BEGIN_NAMESPACE

// The way this camera works:
// The viewing direction of the camera is specified by an EulerAngle and the co-ordinates
// are in editor space, NOT OpenGL space.
// The camera transforms are applied as follows ("the camera is transformed" means the eye and
// centre are transformed where the eye is considered the origin of the transformation):
// - The camera eye is placed at (0,0,0) and the centre is placed at (0,1,0).
// - The camera is rotated around the world Z axis to face the direction
//      specified by yaw. As the yaw increases, the camera rotates anticlockwise around
//      the Z axis as viewed from above.
// - The camera is rotated around its new, local right vector by the value specified in
//      pitch. This value is naively clamped between a maximum range of -89 degrees (pointing
//      up) and 89 degrees (pointing down). If a smaller range has been specified, the pitch
//      is clamped between -delta and delta of this range.
// - The camera is rotated around its new, local viewing vector by the value specified in roll.
//      This ranges from a maximum of 180 degrees (tilted right) to -180 degrees (tilted left).
// - The camera centre is moved the specified number of units along the new viewing vector.
// - The camera eye and centre are moved, maintaining their relative positions, so that the eye
//      resides at the specified eye position.

class VIEWSHARED_EXPORT GLAngleCamera : public QGLCamera
{
    Q_OBJECT
    Q_PROPERTY(float maxPitchDelta READ maxPitchDelta WRITE setMaxPitchDelta NOTIFY maxPitchDeltaChanged)
    Q_PROPERTY(MODEL_NAMESPACE::EulerAngle viewAngles READ viewAngles WRITE setViewAngles)
    Q_PROPERTY(float eyeCenterDistance READ eyeCenterDistance WRITE setEyeCenterDistance NOTIFY eyeCenterDistanceChanged)
    Q_PROPERTY(QVector3D viewVector READ viewVector WRITE setViewVector)
    Q_PROPERTY(QVector3D rightVector READ rightVector)
    Q_PROPERTY(float zoom READ zoom WRITE setZoom)
public:
    explicit GLAngleCamera(QObject *parent = 0);

    void setMaxPitchDelta(float delta);
    float maxPitchDelta() const;

    MODEL_NAMESPACE::EulerAngle viewAngles() const;
    void setViewAngles(const MODEL_NAMESPACE::EulerAngle &angles);

    float eyeCenterDistance() const;
    void setEyeCenterDistance(float distance);

    QVector3D viewVector() const;
    void setViewVector(const QVector3D &dir);

    QVector3D rightVector() const;

    float zoom() const;
    void setZoom(float zoom);

    // Sets the camera eye to pos and keeps the camera centre
    // at the same relative position.
    void setEyeCenterMaintainOffset(const QVector3D &pos);

    // Updates the camera's view angles to reflect the current configuration
    // of the eye and centre. The angles are clamped to restrict pitch etc.
    void updateViewAnglesForCurrentPosition();

    static QVector3D defaultCenter();
    static QVector3D defaultEye();
    static QVector3D defaultUpVector();
    static MODEL_NAMESPACE::EulerAngle defaultViewAngles();
    static QVector3D defaultViewVector();
    void resetPositionAndAngles();

signals:
    void maxPitchDeltaChanged(float);
    void eyeCenterDistanceChanged(float);

public slots:
    void addPitch(float pitch);
    inline void subtractPitch(float pitch) { addPitch(-pitch); }

    void addYaw(float yaw);
    inline void subtractYaw(float yaw) { addYaw(-yaw); }

    void addRoll(float roll);
    inline void subtractRoll(float roll) { addRoll(-roll); }

private:
    void transform(const MODEL_NAMESPACE::EulerAngle &viewAngles, const QVector3D &eyePosition,
                   float eyeCentreDistance);
    void applyEyeCentreDistance();

    MODEL_NAMESPACE::EulerAngle normaliseAngle(const MODEL_NAMESPACE::EulerAngle &angle);

    float                       m_flMaxPitchDelta;
    MODEL_NAMESPACE::EulerAngle m_Angles;
    float                       m_flEyeCentreDistance;
};

VIEW_END_NAMESPACE

#endif // GLANGLECAMERA_H
