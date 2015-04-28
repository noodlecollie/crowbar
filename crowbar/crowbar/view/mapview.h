#ifndef MAPVIEW_H
#define MAPVIEW_H

#include "irender.h"
#include <QMatrix4x4>
#include <QTimer>
#include <QGeometryData>
#include <QOpenGLFunctions>
#include <QScopedPointer>
#include <QGLCameraAnimation>
#include "glanglecamera.h"
#include "mapdocumentview.h"
#include <QBox3D>

#ifdef NEW_WIDGET
#include <QOpenGLWidget>
#define PARENT_CLASSNAME QOpenGLWidget
#else
#include <QGLWidget>
#define PARENT_CLASSNAME QGLWidget
#endif

class QGLSceneNode;
class QGLAbstractEffect;

VIEW_BEGIN_NAMESPACE

class VIEWSHARED_EXPORT MapView : public PARENT_CLASSNAME, protected QOpenGLFunctions
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QMatrix4x4 coordinateTransform READ coordinateTransform WRITE setCoordinateTransform NOTIFY coordinateTransformChanged)
    Q_PROPERTY(GLAngleCamera* camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(GLAngleCamera* defaultCamera READ defaultCamera)
    Q_PROPERTY(QPoint widgetCenter READ widgetCenter)
    Q_PROPERTY(bool drawWhenFocusLost READ drawWhenFocusLost WRITE setDrawWhenFocusLost NOTIFY drawWhenFocusLostChanged)
    Q_PROPERTY(IRender* renderable READ renderable WRITE setRenderable NOTIFY renderableChanged)
    Q_PROPERTY(bool highlightOnFocus READ highlightOnFocus WRITE setHighlightOnFocus NOTIFY highlightOnFocusChanged)
    Q_PROPERTY(bool focusOnMouseOver READ focusOnMouseOver WRITE setFocusOnMouseOver NOTIFY focusOnMouseOverChanged)
    Q_PROPERTY(bool drawSelectionHandles READ drawSelectionHandles WRITE setDrawSelectionHandles NOTIFY drawSelectionHandlesChanged)
public:
    explicit MapView(QWidget* parent = 0, GLAngleCamera* defaultCamera = NULL);
    virtual ~MapView();

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &colour);

    QMatrix4x4 coordinateTransform() const;
    void setCoordinateTransform(const QMatrix4x4 &transform);
    void resetCoordinateTransform();

    GLAngleCamera* camera();
    const GLAngleCamera* camera() const;
    void setCamera(GLAngleCamera* camera);
    void resetCamera();

    GLAngleCamera* defaultCamera();
    const GLAngleCamera* defaultCamera() const;
    
    bool drawWhenFocusLost() const;
    void setDrawWhenFocusLost(bool shouldDraw);
    
    IRender* renderable() const;
    void setRenderable(IRender* renderable);

    virtual QSize minimumSizeHint() const;

    QPoint widgetCenter() const;
    QVector2D widgetCenterF() const;

    bool openGLInitialised() const;

    void setCameraAnimation(QGLCameraAnimation *animation);

    // Keeps the up vector oriented in the same direction as current.
    virtual void moveCameraTo(const QVector3D &pos, float duration = 0.2f,
                              const QEasingCurve &easing = QEasingCurve::InOutCubic);
    
    bool highlightOnFocus() const;
    void setHighlightOnFocus(bool enabled);
    
    // Screen = -1.0f to 1.0f, client = 0 to width/height.
    QVector2D clientToScreen(const QPoint &client);
    QPoint screenToClient(const QVector2D &screen);
    
    // Client co-ordinates
    void setDrawRect(const QRect &rect, const QColor &col = QColor(Qt::white), QGLAbstractEffect* effect = NULL);
    inline void clearDrawRect() { setDrawRect(QRect()); }
    bool hasDrawRect() const;
    
    void setCrosshair(const QPoint &pos, const QColor &col = QColor(Qt::white));
    void clearCrosshair();
    bool hasCrosshair() const;
    
    bool isMouseOverActive() const;

    virtual IRender::RenderMode renderMode() const;
    QList<MODEL_NAMESPACE::MapComponent*> objectsWithin(const QRect &rect);
    MODEL_NAMESPACE::MapFace* faceAt(const QPoint &p);

    bool focusOnMouseOver() const;
    void setFocusOnMouseOver(bool enabled);

    void setSelectionBounds(const QBox3D &bounds);
    bool drawSelectionHandles() const;
    void setDrawSelectionHandles(bool enabled);
    
    bool pickSelectionHandle(const QPoint &p, QString &selectionType, QVector3D &anchor, QVector3D &basePos);
    
signals:
    void coordinateTransformChanged(const QMatrix4x4&);
    void cameraChanged(GLAngleCamera*);
    void backgroundColorChanged(const QColor&);
    void drawWhenFocusLostChanged(bool);
    void renderableChanged(IRender*);
    void highlightOnFocusChanged(bool);
    void viewParamsChanged();
    void painterExposed();
    void focusOnMouseOverChanged(bool);
    void drawSelectionHandlesChanged(bool);

    // Paired signals.
    void cameraAnimationStart(QGLCameraAnimation*);
    void cameraAnimationEnd(QGLCameraAnimation*);

    void dragBegin(const QPoint&);
    void dragMove(const QPoint&);
    void dragEnd();
    
    void rightDragBegin(const QPoint&);
    void rightDragMove(const QPoint&);
    void rightDragEnd();
    
    void mouseOverBegin(const QPoint &pos);
    void mouseOverMove(const QPoint &pos);
    void mouseOverEnd();
    
    void gainedFocus(MapView*);
    void lostFocus(MapView*);

public slots:
    void startCameraAnimation();
    void stopCameraAnimation();

private slots:
    void cameraAnimationStateChanged(QAbstractAnimation::State,QAbstractAnimation::State);
    void buildSelectionHandles();

protected:
    virtual void initializeGL();
    virtual void virtualInitGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

    // Before camera applied - co-ordinates are screen space with (0,0) at centre.
    virtual void prePaintGL(QGLPainter*);

    // After camera applied - co-ordinates are world space.
    virtual void paintGL(QGLPainter*);
    virtual void postPaintGL(QGLPainter*);

    virtual void focusInEvent(QFocusEvent *);
    virtual void focusOutEvent(QFocusEvent *);
    
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
    
    virtual void vDragBegin(const QPoint &pos);
    virtual void vDragMove(const QPoint &pos);
    virtual void vDragEnd();
    
    virtual void vRightDragBegin(const QPoint &pos);
    virtual void vRightDragMove(const QPoint &pos);
    virtual void vRightDragEnd();
    
    bool isBaseDragActive() const;
    bool isRightDragActive() const;

    virtual bool shouldShowSelectionHandles() const { return false; }

private:
    void initialise();
    void setClearColour();
    void buildFocusBorder();
    
    void dragMousePress(const QPoint &pos);
    void dragMouseMove(const QPoint &pos);
    void dragMouseRelease();
    
    void dragRightMousePress(const QPoint &pos);
    void dragRightMouseMove(const QPoint &pos);
    void dragRightMouseRelease();

    static QGLSceneNode* buildSelectionHandle(const QVector2D &lowerLeft, const QVector2D &dimension, const QColor &col);

    QColor                              m_colBackground;
    bool                                m_bGLInitialised;
    QMatrix4x4                          m_matPreRenderTransform;
    GLAngleCamera*                      m_pDefaultCamera;
    GLAngleCamera*                      m_pCamera;
    QTimer                              m_RefreshTimer;
    QScopedPointer<QGLCameraAnimation>  m_pCameraAnimation;
    bool                                m_bDrawWhenFocusLost;
    IRender*                            m_pRenderable;
    bool                                m_bHighlightOnFocus;
    bool                                m_bInDrag;
    bool                                m_bInRightDrag;
    bool                                m_bInMouseOver;
    bool                                m_bFocusOnMouseOver;

    QGLSceneNode*   m_pFocusBorder;
    QGLSceneNode*   m_pRect;
    QGLSceneNode*   m_pCrosshair;

    QBox3D          m_SelectionBounds;
    QGLSceneNode*   m_pSelectionHandles;
    bool            m_bDrawSelectionHandles;
};

VIEW_END_NAMESPACE

#endif // MAPVIEW_H
