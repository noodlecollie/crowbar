#ifndef OPENGLWIDGETV2_H
#define OPENGLWIDGETV2_H

#include <QGLWidget>
#include "qglcamera2.h"
#include <QScopedPointer>
#include <QPointer>
#include <QGLSceneNode>
#include <QTime>
#include <QTimer>
#include <QPoint>
#include <QGLCameraAnimation>
#include "maporiginmarker.h"
#include "mapdocumentrenderer.h"

class QGLLightParameters;
class QGLLightModel;

class OpenGLWidgetV2 : public QGLWidget
{
    Q_OBJECT
    Q_PROPERTY(QGLCamera2* camera READ camera WRITE setCamera)
    Q_PROPERTY(bool initialized READ initialized)
    Q_PROPERTY(float horizontalSensitivity READ horizontalSensitivity WRITE setHorizontalSensitivity)
    Q_PROPERTY(float verticalSensitivity READ verticalSensitivity WRITE setVerticalSensitivity)
public:
    explicit OpenGLWidgetV2(QWidget* parent = 0, const QGLWidget* shareWidget = 0, Qt::WindowFlags f = 0);
    explicit OpenGLWidgetV2(QGLContext* context, QWidget* parent = 0, const QGLWidget* shareWidget = 0, Qt::WindowFlags f = 0);
    explicit OpenGLWidgetV2(const QGLFormat &format, QWidget* parent = 0, const QGLWidget* shareWidget = 0, Qt::WindowFlags f = 0);
    
    ~OpenGLWidgetV2();
    
    void setCamera(QGLCamera2* camera);
    QGLCamera2* camera() const;
    
    bool initialized() const;
    
    void setTransformCoords(bool b);
    bool transformCoords() const;

    virtual QSize minimumSizeHint() const;

    const VIEW_NAMESPACE::MapDocumentRenderer* documentRenderer() const;
    VIEW_NAMESPACE::MapDocumentRenderer* documentRenderer();
    
    void setToRender(const MODEL_NAMESPACE::MapDocument* doc);
    
    float horizontalSensitivity() const;
    void setHorizontalSensitivity(float sensitivity);
    
    float verticalSensitivity() const;
    void setVerticalSensitivity(float sensitivity);
    
    // The widget owns the passed animation.
    void setCameraAnimation(QGLCameraAnimation* animation);
    bool hasCameraAnimation() const;
    inline void cancelCameraAnimation() {setCameraAnimation(NULL);}

    void setTempBrush(MODEL_NAMESPACE::MapBrush* brush) { m_pTempBrush = brush; }

    void setHighlight(bool enabled);
    bool highlight() const;
    void setHighlightWidth(float width);
    float highlightWidth() const;
    
signals:
    
public slots:
    
private slots:
    void animationFinished();
    
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);
    virtual void focusInEvent(QFocusEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    
private:
    void init();
    void processMouseMovement(const QPoint &pos);
    void moveCursorToWidgetCentre();
    QPoint widgetCentre() const;
    void toggleMouseGrab();
    
    QScopedPointer<QGLCamera2>      m_pDefaultCamera;
    QPointer<QGLCamera2>            m_pCamera;
    bool                            m_bInitialised;
    QTime                           m_FrameTimer;
    QTimer                          m_RefreshTimer;
    
    bool                            m_bGrabMouse;
    float                           m_flHSensitivity;
    float                           m_flVSensitivity;

    VIEW_NAMESPACE::MapDocumentRenderer*    m_pDocumentRenderer;
    QGLSceneNode*                           m_pOrigin;
    QGLLightParameters*                     m_pLighting;
    QGLLightModel*                          m_pGlobalLighting;
    
    QScopedPointer<QGLCameraAnimation> m_pCameraAnimation;
    bool                            m_bDisableCameraControl;    // Temporarily disable camera control while the camera is animating.
    
    bool                            m_bTransformCoordinateSystem;

    MODEL_NAMESPACE::MapBrush*      m_pTempBrush;

    bool                            m_bHighlightOnFocus;
    float                           m_flHighlightWidth;
    QGeometryData                   m_HighlightData;
};

#endif // OPENGLWIDGETV2_H
