#ifndef IRENDERDATA_H
#define IRENDERDATA_H

#include "irenderdata_global.h"
#include <QObject>
#include <QList>
#include <Qt3D/qglnamespace.h>
#include <Qt3D/QGeometryData>

class IRenderDataGroup;
class QGLPainter;

namespace IRenderData
{
    class IRENDERDATASHARED_EXPORT IRenderData : public QObject
    {
        Q_OBJECT
    public:
        
        // Data groups stored, eg. brushes, displacements, models, etc.
        // Geometry is drawn in ascending index order.
        enum DataGroups
        {
            GroupDebug = 0,     // Debug test geometry to be rendered.
            GROUPS_MAX          // Number of data groups registered.
        };
        
        // Render categories. Each of these requires a new render pass.
        // Categories are drawn in ascending index order.
        enum RenderCategories
        {
            CatMain = 0,    // Main bulk of rendering - normal sorting, opaque.
            CatTranslucent, // Translucent objects
            CatOverlay,     // Overlays
            CAT_MAX
        };
        
        // This is a render category - a list of pointers to QGeometryData objects.
        // A data group entry should include CAT_MAX of these lists in an array, each index
        // corresponding to a RenderCategories enum value;
        typedef QList<QGeometryData*> RenderCategory;
        
        // Constructor, destructor.
        IRenderData(QObject *parent = 0);
        ~IRenderData();
        
        // Draw geometry with the given painter, from the given data group and render category.
        // If group = GROUPS_MAX, all data groups are drawn.
        // If category = CAT_MAX, all categories are drawn (in the correct order).
        // The rest of the arguments mirror QGeometryData::draw();
        void draw(QGLPainter* painter, int start, int count, DataGroups group = GROUPS_MAX, RenderCategories category = CAT_MAX,
                  GLenum mode = QGL::Triangles, qreal drawWidth = 1.0);
        
    private:
        QList<IRenderDataGroup*>    m_pDataGroups;
    };
    
    class IRENDERDATASHARED_EXPORT IRenderDataGroup : public QObject
    {
        Q_OBJECT
    public:
        IRenderDataGroup(QObject *parent = 0);
        ~IRenderDataGroup();
        
        // Array of render categories. These are set to NULL on construction and deleted (if not NULL) on destruction.
        IRenderData::RenderCategory*    categories[IRenderData::CAT_MAX];
    };
}

#endif // IRENDERDATA_H
