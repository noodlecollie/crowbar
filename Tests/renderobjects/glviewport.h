#ifndef GLVIEWPORT_H
#define GLVIEWPORT_H

#include <QGLWidget>

class QGLPainter;
class QGLAbstractSurface;
class QGLCamera;

class GLViewport : public QGLWidget
{
    Q_OBJECT
    Q_PROPERTY(QGLCamera* camera READ camera WRITE setCamera)
public:
    explicit GLViewport(QWidget* parent = 0, const QGLWidget* shareWidget = 0, Qt::WindowFlags f = 0);
    explicit GLViewport(QGLContext* context, QWidget* parent = 0, const QGLWidget* shareWidget = 0, Qt::WindowFlags f = 0);
    explicit GLViewport(const QGLFormat &format, QWidget* parent = 0, const QGLWidget* shareWidget = 0, Qt::WindowFlags f = 0);
    
    ~GLViewport();
    
    QGLCamera* camera() const;
    void setCamera(QGLCamera* camera);
    
signals:
    
public slots:
    
protected:
    
    // Re-implemented from QGLWidget.
    // Note: for threaded rendering, we will probably need to create a single OpenGL thread and have
    // paintGL() do nothing, allowing the thread to do the painting instead.
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    
    // GL functions referencing a specific QGLPainter.
    // As far as I can see these functions are more convenient versions of their above counterparts
    // because they remove some of the boilerplate code.
    virtual void initializeGL(QGLPainter* painter);
    virtual void earlyPaintGL(QGLPainter* painter);
    virtual void paintGL(QGLPainter* painter);
    
private:
    bool                    m_bInitialised;
    
    // TODO: Which pointer class should we use for these?
    QGLAbstractSurface*     m_pMainSurface;
    QGLCamera*              m_pDefaultCamera;
    QGLCamera*              m_pCamera;
};

#endif // GLVIEWPORT_H
