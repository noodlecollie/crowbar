#ifndef MAPVIEWGRID_H
#define MAPVIEWGRID_H

#include "view_global.h"
#include <QObject>
#include <QSize>
#include <QColor>
#include <QVector3D>
#include <QVector2D>
#include <QGeometryData>
#include <QMatrix4x4>
#include <QVector4D>

#define GRID_ZOOM_MIN   0.01f
#define GRID_ZOOM_MAX   256.0f

class QGLSceneNode;

VIEW_BEGIN_NAMESPACE

class MapView2D;

class VIEWSHARED_EXPORT MapViewGrid : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor majorColor READ majorColor WRITE setMajorColor NOTIFY majorColorChanged)
    Q_PROPERTY(QColor minorColor READ minorColor WRITE setMinorColor NOTIFY minorColorChanged)
    Q_PROPERTY(QColor originColor READ originColor WRITE setOriginColor NOTIFY originColorChanged)
    Q_PROPERTY(QColor standardColor READ standardColor WRITE setStandardColor NOTIFY standardColorChanged)
    Q_PROPERTY(int gridPower READ gridPower WRITE setGridPower NOTIFY gridPowerChanged)
    Q_PROPERTY(QVector2D center READ center WRITE setCenter NOTIFY centerChanged)
    Q_PROPERTY(float zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(float minPixelSpacing READ minPixelSpacing WRITE setMinPixelSpacing NOTIFY minPixelSpacingChanged)
public:
    explicit MapViewGrid(QObject *parent = 0);

    // Major gridlines = lines on 1024-unit boundaries. Always drawn unless an origin
    // line has been drawn.
    QColor majorColor() const;
    void setMajorColor(const QColor &col);

    // Minor gridlines = lines on 64-unit boundaries but adhere to grid size.
    // Not drawn if origin or major gridlines have been drawn, and
    QColor minorColor() const;
    void setMinorColor(const QColor &col);

    // Lines where any axis value = 0. Always drawn.
    QColor originColor() const;
    void setOriginColor(const QColor &col);

    // Standard lines - gridlines on 1-unit boundaries but adhere to grid size.
    // Not drawn if any other lines have already been drawn.
    QColor standardColor() const;
    void setStandardColor(const QColor &col);

    // Minimum grid spacing to draw.
    // For example, gridPower of 3 would draw gridlines with 2^3 spacing = 8 units apart.
    int gridPower() const;
    void setGridPower(int power);

    // Specifies the position of the centre of the view on the grid.
    // For example, a value of (0,0) means the origin is at the centre of the view.
    // A value of (2,3) means point (2,3) is at the centre of the view, etc.
    QVector2D center() const;
    void setCenter(const QVector2D &centre);

    float zoom() const;
    void setZoom(float z);
    
    float minPixelSpacing() const;
    void setMinPixelSpacing(float spacing);

    // Returns a scene node representing the grid in screen space.
    QGLSceneNode* grid(const MapView2D* view) const;
    void draw(QGLPainter* painter, const MapView2D* view);
    
    // View co-ordinates use (0,0) as the centre of the view.
    QVector2D viewToGrid(const QVector2D &vec) const;   // < Ignore grid translation
    QVector2D gridToView(const QVector2D &vec) const;   // <

    QVector2D gridToViewFull(const QVector2D &vec) const;   // < Include grid translation
    QVector2D viewToGridFull(const QVector2D &vec) const;   // <

    QVector2D viewToScreen(const QVector2D &vec) const;
    QVector2D screenToView(const QVector2D &vec) const;

signals:
    void majorColorChanged(const QColor&);
    void minorColorChanged(const QColor&);
    void originColorChanged(const QColor&);
    void standardColorChanged(const QColor&);
    void gridPowerChanged(int);
    void centerChanged(const QVector2D&);
    void zoomChanged(float);
    void minPixelSpacingChanged(float);

public slots:
    void queueUpdate();

private:
    QGLSceneNode* buildNode(const MapView2D* view) const;
    QGeometryData gridlines(const MapView2D* view) const;
    QColor colourForCoordinate(int i) const;
    void regenerateTransforms();
    
    // Combine both of these for the full transform.
    static QMatrix4x4 viewToGridTransform(const QVector3D &translation);
    static QMatrix4x4 viewToGridTransform(float zoom);
    static QVector4D vec2ToVec4(const QVector2D &vec);
    static QVector2D vec4ToVec2(const QVector4D &vec);

    // min and max are in (2D) world space.
    // gridSpacing specified how many world units there are between grid lines.
    // lowerBound and upperBound specify the minimum and maximum grid points that are contained
    // between min and max (ie. >= min and <= max).
    static void lowerUpperGridBounds(const QVector2D &min, const QVector2D &max, int gridSpacing,
                                QPoint &lowerBound, QPoint &upperBound);

    int             m_iPowerTwo;
    QColor          m_colMajor;
    QColor          m_colMinor;
    QColor          m_colOrigin;
    QColor          m_colStd;
    
    QMatrix4x4      m_matGridCentre;
    
    QMatrix4x4      m_matZoom;
    
    QMatrix4x4      m_matViewGridTransform;
    QMatrix4x4      m_matGridViewTransform;
    
    float           m_flMinPixelSpacing;
    
    mutable bool    m_bNeedsUpdate;
    mutable QSize   m_Size;
    mutable QGLSceneNode*   m_pNode;
};

VIEW_END_NAMESPACE

#endif // MAPVIEWGRID_H
