#ifndef MAPFACE_H
#define MAPFACE_H

#include "model_global.h"
#include "mapcomponent.h"
#include "imaphandle.h"
#include <QVector>
#include <QVector3D>
#include "mapvertex.h"

MODEL_BEGIN_NAMESPACE

class MapVertex;
class MapDocument;

class MODELSHARED_EXPORT MapFace : public MapComponent
{
    Q_OBJECT
public:
    typedef QVector<MapVertex*> VertexVector;
    
    explicit MapFace(QObject *parent = 0);
    explicit MapFace(MapVertex** vertices, int count, QObject* parent = 0);
    explicit MapFace(const MapVertexList &list, QObject* parent = 0);
    
    void addVertex(MapVertex* vertex);
    void removeVertex(MapVertex* vertex);
    MapVertex* removeVertexAt(int index);
    int indexOfVertex(MapVertex* vertex) const;
    int vertexCount() const;
    bool containsVertex(MapVertex* vertex) const;
    void clearVertices();
    MapVertex* vertexAt(int index);
    const MapVertex* vertexAt(int index) const;
    
    QVector3D normal() const;

    MapDocument* document();
    const MapDocument* document() const;
    void setDocument(MapDocument* doc);
    
signals:
    void faceRenderDataChanged();
    
public slots:
    
private:
    VertexVector    m_Vertices;
    MapDocument*    m_pDocument;
};

MODEL_END_NAMESPACE

#endif // MAPFACE_H
