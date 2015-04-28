#ifndef MAPEDGE_H
#define MAPEDGE_H

#include "model_global.h"
#include "mapcomponent.h"
#include "imaphandle.h"
#include "mapvertex.h"

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT MapEdge : public MapComponent
{
    Q_OBJECT
    Q_PROPERTY(MapVertex* vertex0 READ vertex0 WRITE setVertex0 NOTIFY vertex0Changed)
    Q_PROPERTY(MapVertex* vertex1 READ vertex1 WRITE setVertex1 NOTIFY vertex1Changed)
public:
    explicit MapEdge(QObject *parent = 0);
    explicit MapEdge(MapVertex* v0, MapVertex* v1, QObject* parent = 0);
    
    void setVertex0(MapVertex* vertex);
    void setVertex1(MapVertex* vertex);
    void setVertex(int index, MapVertex* vertex);
    MapVertex* vertex0();
    const MapVertex* vertex0() const;
    MapVertex* vertex1();
    const MapVertex* vertex1() const;

    MapDocument* document();
    const MapDocument* document() const;
    void setDocument(MapDocument* doc);
    
signals:
    void vertex0Changed(const MODEL_NAMESPACE::MapVertex*);
    void vertex1Changed(const MODEL_NAMESPACE::MapVertex*);
    void edgeRenderDataChanged();
public slots:
    
private:
    MapVertex*      m_pVertices[2];
    MapDocument*    m_pDocument;
};

MODEL_END_NAMESPACE

#endif // MAPEDGE_H
