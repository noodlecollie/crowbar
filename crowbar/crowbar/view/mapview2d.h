#ifndef MAPVIEW2D_H
#define MAPVIEW2D_H

#include "view_global.h"
#include "mapview.h"
#include "mapviewgrid.h"
#include <QMatrix3x3>
#include <QVector2D>
#include <QPoint>

#define CAMERA2D_NEAR_PLANE_OFFSET 5.0f

VIEW_BEGIN_NAMESPACE

class VIEWSHARED_EXPORT MapView2D : public MapView
{
    Q_OBJECT
    Q_PROPERTY(float zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(float minGridPixelSpacing READ minGridPixelSpacing WRITE setMinGridPixelSpacing NOTIFY minGridPixelSpacingChanged)
    Q_PROPERTY(int gridPower READ gridPower WRITE setGridPower NOTIFY gridPowerChanged)
    Q_PROPERTY(float zoomSensitivity READ zoomSensitivity WRITE setZoomSensitivity NOTIFY zoomSensitivityChanged)
    Q_PROPERTY(QVector2D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(bool drawGrid READ drawGrid WRITE setDrawGrid NOTIFY drawGridChanged)
    Q_ENUMS(CameraDirection)
public:
    enum CameraDirection
    {
        XPos = 0,   // Camera looks down +X, up vector +Z *
        XNeg,       // Camera looks down -X, up vector +Z
        YPos,       // Camera looks down +Y, up vector +Z
        YNeg,       // Camera looks down -Y, up vector +Z *
        ZPos,       // Camera looks down +Z, up vector +Y *
        ZNeg,       // Camera looks down -Z, up vector +Y
        
        // Below are for convenience - match Hammer.
        // Starred directions above end up with unintuitive 2D axes -
        // we're used to right/up directions being positive, which is
        // not the case for the starred options.
        // Hence, below retain intiutive axes.
        Top = ZNeg,
        Side = YPos,
        Front = XNeg,
    };
    
    explicit MapView2D(QWidget* parent = 0, GLAngleCamera* defaultCamera = NULL);
    virtual ~MapView2D();

    float zoom() const;
    void setZoom(float zoom);

    void zoomIn(float scaleFactor);
    inline void zoomOut(float scaleFactor) { zoomIn(1.0f / scaleFactor); }
    
    float minGridPixelSpacing() const;
    void setMinGridPixelSpacing(float spacing);
    
    int gridPower() const;
    void setGridPower(int power);

    float zoomSensitivity() const;
    void setZoomSensitivity(float s);
    
    CameraDirection direction() const;
    void setDirection(CameraDirection direction);
    
    QVector2D position() const;
    void setPosition(const QVector2D &pos);
    
    bool drawGrid() const;
    void setDrawGrid(bool enabled);

    bool circularCenterHandles() const;
    void setCircularCenterHandles(bool enabled);
    
    static QVector3D gridXAxisInWorld(CameraDirection dir);
    static QVector3D gridYAxisInWorld(CameraDirection dir);
    static QVector3D gridZAxisInWorld(CameraDirection dir);
    static QVector3D gridViewAxisInWorld(CameraDirection dir);
    
    static QMatrix4x4 gridToWorld(CameraDirection dir);      // Construct a matrix to convert from grid co-ordinates to world co-ordinates for the given camera direction.
    inline QMatrix4x4 gridToWorld() const { return gridToWorld(direction()); }

    // Translates the grid so that the given window pixel resides at the given grid co-ordinate.
    __MARK_DEPRECATED void translateGrid(const QPoint &p, const QVector2D &gridPos);
    
    bool isDragActive() const;

    virtual IRender::RenderMode renderMode() const;

signals:
    void zoomChanged(float);
    void minGridPixelSpacingChanged(float);
    void gridPowerChanged(int);
    void renderableChanged(IRender*);
    void zoomSensitivityChanged(float);
    void directionChanged(CameraDirection);
    void positionChanged(const QVector2D&);
    void drawGridChanged(bool);
    void circularCenterHandlesChanged(bool);
    
private slots:
    void updateCamera();

protected:
    virtual void virtualInitGL();
    virtual void prePaintGL(QGLPainter *);
    virtual void paintGL(QGLPainter *);
    virtual void postPaintGL(QGLPainter *);

    virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);
    virtual void wheelEvent(QWheelEvent *);

    virtual void resizeGL(int w, int h);
    
    virtual void vDragBegin(const QPoint &pos);
    virtual void vDragMove(const QPoint &pos);
    virtual void vDragEnd();
    
    virtual void vRightDragBegin(const QPoint &pos);
    virtual void vRightDragMove(const QPoint &pos);
    virtual void vRightDragEnd();

    virtual bool shouldShowSelectionHandles() const { return true; }

private:
    void initialise();
    void cameraParameters(QVector3D &eye, QVector3D &center, QVector3D &up);

    void scrollDragMousePress(const QPoint &pos);
    void scrollDragMouseMove(const QPoint &pos);
    void scrollDragMouseRelease();

    // Translate the grid so that the given client point resides at the given grid point.
    void putClientPositionOnGrid(const QPoint &client, const QVector2D &gridTarget);

    MapViewGrid     m_Grid;
    float           m_flZoomSensitivity;
    CameraDirection m_iDirection;
    bool            m_bDrawGrid;
    bool            m_bDragToScroll;
    bool            m_bInScrollDrag;
    QPoint          m_LastDragPos;
};

VIEW_END_NAMESPACE

#endif // MAPVIEW2D_H
