#include "openglwidget.h"

// OLD CODE
#if 0
OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
}

OpenGLWidget::~OpenGLWidget()
{

}

void OpenGLWidget::initializeGL()
{
    if ( !m_bInitialiserSet ) return;
    // Set clear colour and clear depth buffer.
    qglClearColor(m_Initialiser.m_ClearColour);
    glClearDepth(1.0);
    glDepthFunc(m_Initialiser.m_iDepthFunc);

    glShadeModel(m_Initialiser.m_iShadeModel);

    // Enable each thing on our list.
    for ( int i = 0; i < m_Initialiser.m_iEnables.size(); i++ )
    {
        glEnable(m_Initialiser.m_iEnables.at(i));
    }

    // Set all hints on our list.
    for ( int i = 0; i < m_Initialiser.m_Hints.size(); i++ )
    {
        glHint(m_Initialiser.m_Hints.at(i).first, m_Initialiser.m_Hints.at(i).second);
    }
}

void OpenGLWidget::resizeGL(int width, int height)
{
    // Run this first in subclasses, then set the projection matrix (2D/3D,
    // orthographic/perspective).
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}
#endif // 0
