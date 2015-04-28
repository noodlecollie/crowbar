#ifndef OPENGLCAMERAANIMATION_H
#define OPENGLCAMERAANIMATION_H

#include <QGLCameraAnimation>
#include <QGLCamera>

// Note - don't actually need this subclass! We can just use the pause/resume/etc. slots on QAbstractAnimation.

class OpenGLCameraAnimation : public QGLCameraAnimation
{
    Q_OBJECT
    Q_PROPERTY(float position READ position WRITE setPosition)
public:
    explicit OpenGLCameraAnimation(QObject *parent = 0);
    
    float position() const;
    
    // If the current time is equal to the duration, the animation is considered to be finished.
    bool isFinished() const;
    
    inline void setStartFromCamera(const QGLCamera &camera);
    inline void setEndFromCamera(const QGLCamera &camera);
    
signals:
    
public slots:
    void setPosition(float pos);    // 0.0f = start, 1.0f = end
    inline void resetPosition() {setPosition(0.0f);}
    void advanceAnimation(int dmsec);
    
private:
    void init();
    
    int m_iCurrentTime; // Holds the current amount of time that has passed from each call to updateCurrentTime().
};

inline void OpenGLCameraAnimation::setStartFromCamera(const QGLCamera &camera)
{
    setStartCenter(camera.center());
    setStartEye(camera.eye());
    setStartUpVector(camera.upVector());
}

inline void OpenGLCameraAnimation::setEndFromCamera(const QGLCamera &camera)
{
    setEndCenter(camera.center());
    setEndEye(camera.eye());
    setEndUpVector(camera.upVector());
}


#endif // OPENGLCAMERAANIMATION_H
