#include "openglcameraanimation.h"

OpenGLCameraAnimation::OpenGLCameraAnimation(QObject *parent) :
    QGLCameraAnimation(parent)
{
    init();
}

void OpenGLCameraAnimation::init()
{
    m_iCurrentTime = 0;
}

void OpenGLCameraAnimation::advanceAnimation(int dmsec)
{
    // Advance our current time.
    m_iCurrentTime += dmsec;
    
    // Call updateCurrentTime() with the new time value.
    updateCurrentTime(m_iCurrentTime);
}

void OpenGLCameraAnimation::setPosition(float pos)
{
    // Update current time to pos * duration of animation.
    m_iCurrentTime = qRound(pos * (float)duration());
    updateCurrentTime(m_iCurrentTime);
}

float OpenGLCameraAnimation::position() const
{
    if ( duration() == 0 ) return 0.0f;
    return (float)m_iCurrentTime / (float)duration();
}

bool OpenGLCameraAnimation::isFinished() const
{
    return m_iCurrentTime >= duration();
}
