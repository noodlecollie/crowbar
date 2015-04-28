#ifndef ICAMERA3D_H
#define ICAMERA3D_H

class CameraAngles;
class QVector3D;
class QMatrix4x4;
class CameraFrustum;
class QBox3D;

class ICamera3D
{
    virtual ~ICamera3D() {}
    
    // Angles
    /**
     * @brief Sets the angles of the camera eye The centre is translated to the correct position and
     * the relative distance between the eye and the centre is maintained.
     * @param angles Angles to set.
     */
    virtual void setEyeAngles(const CameraAngles &angles) = 0;
    
    /**
     * @brief Gets the camera eye's angles.
     * @return Eye angles.
     */
    virtual CameraAngles eyeAngles() const = 0;
    
    /**
     * @brief Sets the angles of the camera centre. The eye is translated to the correct position and
     * the relative distance between the eye and the centre is maintained.
     * @param angles Angles to set.
     */
    virtual void setCenterAngles(const CameraAngles &angles) = 0;
    
    /**
     * @brief Gets the camera centre's angles.
     * @return Centre angles.
     */
    virtual CameraAngles centerAngles() const = 0;
    
    // Mouselook controls
    /**
     * @brief Tilts the camera eye. If \a degrees is positive the camera tilts down; otherwise it tilts up.
     * 
     * The tilt is performed around the projection of the camera's eyeRightVector() perpendicular to the world Z axis,
     * and is limited to eyeMaxPitchDelta() degrees from horizontal in each direction. The camera centre is translated
     * to the appropriate position and the relative distance between the eye and the centre is maintained.
     * @param degrees Degrees to tilt.
     */
    virtual void tiltEye(float degrees)  = 0;   // Camera space, moves centre
    
    /**
     * @brief Pans the camera eye. If \a degrees is positive the camera rotates anticlockwise; otherwise it rotates clockwise.
     * 
     * The pan is performed around the world Z axis. The camera centre is translated to the appropriate position and the relative
     * distance between the eye and the centre is maintained.
     * @param degrees Degrees to pan.
     */
    virtual void panEye(float degrees) = 0;     // World space, moves centre
    
    /**
     * @brief Rolls the camera eye. If \a degrees is positive the camera rolls left; otherwise it rolls right.
     * 
     * The roll is performed around the camera's eyeForwardVector() and is limited to eyeMaxRollDelta() from vertical in each direction.
     * The centre is not translated.
     * @param degrees Degrees to roll.
     */
    virtual void rollEye(float degrees) = 0;    // Camera space, does not move centre
    
    // Orbit controls
    /**
     * @brief Tilts the camera centre. If \a degrees is positive the centre tilts down; otherwise it tilts up.
     * 
     * The tilt is performed around the axis perpendicular to the centre's "right" vector, ie. the vector perpendicular to the world
     * Z axis and the vector between the eye and the centre, and is limited to eyeMaxPitchDelta() degrees from horizontal in each
     * direction. The camera eye is translated to the appropriate position and the relative distance between the eye and the centre
     * is maintained.
     * @param degrees Degrees to tilt.
     */
    virtual void tiltCenter(float degrees) = 0; // World space, moves eye
    
    /**
     * @brief Pans the camera centre. If \a degrees is positive then the centre rotates anticlockwise; otherwise it rotates clockwise.
     * 
     * The pan is performed around the world Z axis. The camera eye is translated to the appropriate position and the relative
     * distance between the eye and the centre is maintained.
     * @param degrees Degrees to pan.
     */
    virtual void panCenter(float degrees) = 0;  // World space, moves eye
    
    // Position
    // These can just be mapped into the setEye() and setCenter() functions of QGLCamera.
    /**
     * @brief Sets the camera eye's position in world space.
     * @param pos Position to set.
     */
    virtual void setEyePosition(const QVector3D &pos) = 0;
    
    /**
     * @brief Returns the camera's eye position in world space.
     * @return Position of the eye.
     */
    virtual QVector3D eyePosition() const = 0;
    
    /**
     * @brief Sets the camera centre's position in world space.
     * @param pos Position to set.
     */
    virtual void setCenterPosition(const QVector3D &pos) = 0;
    
    /**
     * @brief Returns the camera's centre position in world space.
     * @return 
     */
    virtual QVector3D centerPosition() const = 0;
    
    // Relative motion controls
    /**
     * @brief Moves the camera eye forward the specified number of units.
     * This is along the positive eyeForwardVector().
     * @param units Units to move forward.
     */
    virtual void moveEyeForward(float units) = 0;   // +Z axis
    
    /**
     * @brief Moves the camera eye right the specified number of units.
     * This is along the positive eyeRightVector().
     * @param units Units to move right.
     */
    virtual void moveEyeRight(float units) = 0;     // +X axis
    
    /**
     * @brief Moves the camera eye up the specified number of units.
     * This is along the positive eyeUpVector().
     * @param units Units to move up.
     */
    virtual void moveEyeUp(float units) = 0;        // +Y axis
    
    // Camera vectors - should be pre-normalised.
    /**
     * @brief Returns a normalised vector representing the camera eye's positive forward vector.
     * @return Forward vector.
     */
    virtual QVector3D eyeForwardVector() const = 0;
    
    /**
     * @brief Returns a normalised vector representing the camera eye's positive up vector.
     * @return Up vector.
     */
    virtual QVector3D eyeUpVector() const = 0;
    
    /**
     * @brief Returns a normalised vector representing the camera eye's positive right vector.
     * @return Right vector.
     */
    virtual QVector3D eyeRightVector() const = 0;
    
    // Pitch limiting
    /**
     * @brief Sets the maximum pitch deviation from horizontal, in degrees.
     * @param degrees Mamimum delta.
     */
    virtual void setEyeMaxPitchDelta(float degrees) = 0;
    
    /**
     * @brief Returns the maximum pitch deviation from horizontal.
     * @return Maximum delta.
     */
    virtual float eyeMaxPitchDelta() const = 0;
    
    // Roll limiting
    /**
     * @brief Sets the maximum roll deviation from vertical, in degrees.
     * @param degrees Maximum delta.
     */
    virtual void setEyeMaxRollDelta(float degrees) = 0;
    
    /**
     * @brief Returns the maximum roll delta from vertical.
     * @return Maximum delta.
     */
    virtual float eyeMaxRollDelta() const = 0;
    
    // Field of view
    /**
     * @brief Sets the camera's field of view. Should be greater than 0.
     * 
     * The field of view is measured in degrees
     * @param degrees FOV in degrees.
     */
    virtual void setFov(float degrees) = 0;
    
    /**
     * @brief Returns the camera's field of view.
     * @return FOV in degrees.
     */
    virtual float fov() const = 0;
    
    // Matrices
    /**
     * @brief Returns the camera's projection matrix for the given aspect ratio.
     * @param aspectRatio If greater than 1, the window is wider than it is high; if less than 1, the window is higher than it is wide.
     * @return Projection matrix.
     */
    virtual QMatrix4x4 projectionMatrix(float aspectRatio) const = 0;
    
    /**
     * @brief Returns the camera's model view matrix.
     * @return Model view matrix.
     */
    virtual QMatrix4x4 modelViewMatrix() const = 0;
    
    // Frustum and bounds.
    /**
     * @brief Returns the camera frustum.
     * @return Camera frustum.
     */
    virtual CameraFrustum viewFrustum() const = 0;
    
    /**
     * @brief Returns the bounding box of the camera frustum.
     * @return Frustum bounding box.
     */
    virtual QBox3D viewBounds() const = 0;
};

#endif // ICAMERA3D_H
