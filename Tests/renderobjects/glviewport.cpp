#include "glviewport.h"
#include <QOpenGLFunctions>
#include <QGLPainter>
#include <QGLAbstractSurface>

GLViewport::GLViewport(const QGLFormat &format, QWidget *parent, const QGLWidget *shareWidget, Qt::WindowFlags f)
    : QGLWidget(format, parent, shareWidget, f), m_bInitialised(false), m_pMainSurface(NULL), m_pDefaultCamera(new QGLCamera(this)),
      m_pCamera(m_pDefaultCamera)
{
    
}

GLViewport::GLViewport(QGLContext *context, QWidget *parent, const QGLWidget *shareWidget, Qt::WindowFlags f)
    : QGLWidget(context, parent, shareWidget, f), m_bInitialised(false), m_pMainSurface(NULL), m_pDefaultCamera(new QGLCamera(this)),
      m_pCamera(m_pDefaultCamera)
{
    
}

GLViewport::GLViewport(QWidget *parent, const QGLWidget *shareWidget, Qt::WindowFlags f)
    : QGLWidget(parent, shareWidget,f), m_bInitialised(false), m_pMainSurface(NULL), m_pDefaultCamera(new QGLCamera(this)),
      m_pCamera(m_pDefaultCamera)
{
    
}

GLViewport::~GLViewport()
{
    if ( m_pCamera )
    {
        delete m_pCamera;
        m_pCamera = NULL;
    }
    
    if ( m_pDefaultCamera )
    {
        delete m_pDefaultCamera;
        m_pDefaultCamera = NULL;
    }
}

QGLCamera* GLViewport::camera() const
{
    return m_pCamera ? m_pCamera : m_pDefaultCamera;
}

void GLViewport::setCamera(QGLCamera *camera)
{
    m_pCamera = camera;
}

void GLViewport::initializeGL()
{
    // For now these are being hard-coded - work out the best way to deal with them later.
    // This is pretty much copied line for line from QGLView.
    
    // Enable depth testing.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
#if defined(QT_OPENGL_ES)
    glDepthRangef(0.0f, 1.0f);
#else
    glDepthRange(0.0f, 1.0f);
#endif
    
    // Set the default blend options.
    // We can get some params from the QGLPainter.
    QGLPainter painter;
    if (painter.hasOpenGLFeature(QOpenGLFunctions::BlendColor))
        painter.glBlendColor(0, 0, 0, 0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (painter.hasOpenGLFeature(QOpenGLFunctions::BlendEquation))
        painter.glBlendEquation(GL_FUNC_ADD);
    else if (painter.hasOpenGLFeature(QOpenGLFunctions::BlendEquationSeparate))
        painter.glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    
    // We should have face culling on!
    // Dunno why QGLView has it off.
    glEnable(GL_CULL_FACE);
    
    // TODO: Create the main surface.
    // What type of surface should we create here??
    
    // Get the size of the geometry and resize the OpenGL display.
    QRect r = geometry();
    resizeGL(r.width(), r.height());
    
    // Do any initialisation with the painter.
    initializeGL(&painter);
    
    // Flag us as fully initialised.
    m_bInitialised = true;
}

void GLViewport::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLViewport::paintGL()
{
    // If we aren't initialised, do so.
    if ( m_bInitialised ) initializeGL();
    
    // If we do not have a surface, return;
    if ( !m_pMainSurface ) return;
    
    // Create a painter;
    QGLPainter painter;
    
    
    // This is not paired with a call to painter.end() in QGLView::paintGL(),
    // which I'm assuming is for efficiency reasons.
    painter.begin();
    
    // Push the main surface.
    painter.pushSurface(m_pMainSurface);
    
    // Do any early painting.
    earlyPaintGL(&painter);
    
    // Set the camera.
    painter.setCamera(camera());
    
    // Do standard painting.
    paintGL(&painter);
    
    // Pop the main surface.
    painter.popSurface();
}

void GLViewport::initializeGL(QGLPainter *painter)
{
    
}

void GLViewport::earlyPaintGL(QGLPainter *painter)
{
    
}

void GLViewport::paintGL(QGLPainter *painter)
{
    
}
