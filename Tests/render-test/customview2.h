#ifndef CUSTOMVIEW2_H
#define CUSTOMVIEW2_H

#include <Qt3D/QGLView>
#include "worldculltreenode.h"
#include <QHash>
#include <Qt3D/QGeometryData>
#include "iconstbboxvolume.h"

class GeomDataEx : public QGeometryData, public GEOMETRY_NAMESPACE::IConstBBoxVolume
{
public:
    explicit GeomDataEx() : QGeometryData() {}
    GeomDataEx(const GeomDataEx &other) : QGeometryData(other) {}
    GeomDataEx(const QGeometryData &other) : QGeometryData(other) {}
    explicit GeomDataEx(qint32 fields) : QGeometryData(fields) {}
    ~GeomDataEx() {}
    virtual QBox3D boundingBox() const { return QGeometryData::boundingBox(); }
};

class CustomView2 : public QGLView
{
public:
    CustomView2(QWindow *parent = 0) : QGLView(parent), m_pCullTreeNode(NULL) {}
    ~CustomView2();
    
    typedef DATASTRUCTURES_NAMESPACE::WorldCullTreeNode<GeomDataEx*> CullTreeNode;
    
    void setNode(CullTreeNode* node) { m_pCullTreeNode = node; }
    CullTreeNode* node() const { return m_pCullTreeNode; }
    
protected:
    void initializeGL(QGLPainter *painter);
    void paintGL(QGLPainter *painter);
    
private:
    void renderNode(QGLPainter* painter, CullTreeNode* node, QHash<GeomDataEx*,char> &prevRendered);
    CullTreeNode*  m_pCullTreeNode;
};

#endif // CUSTOMVIEW2_H
