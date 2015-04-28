#ifndef MAPFACE_H
#define MAPFACE_H

#include "positionalsubcomponent.h"
#include "model_global.h"
#include "shared_vectors.h"
#include <QVector>
#include <QVector3D>
#include "mapvertex.h"
#include "facetextureinfo.h"
#include <QPlane3D>

MODEL_BEGIN_NAMESPACE

class FaceTextureInfo;

class MODELSHARED_EXPORT MapFace : public PositionalSubComponent
{
    Q_OBJECT
    Q_PROPERTY(bool autoUpdateTextureAxes READ autoUpdateTextureAxes WRITE setAutoUpdateTextureAxes NOTIFY autoUpdateTextureAxesChanged)
public:
    typedef Vectors::ExposedQPVector_MapVertex VertexVector;

    explicit MapFace(MapComponent *parent = 0);

    VertexVector& vertices();
    const VertexVector& vertices() const;
    MapVertex* vertices(int index);
    const MapVertex* vertices(int index) const;

    virtual QBox3D boundingBox() const;
    virtual QVector3D centroid() const;
    virtual bool isValid() const;

    QVector3D normal() const;
    QVector3D globalNormal() const;

    FaceTextureInfo* textureInfo();
    const FaceTextureInfo* textureInfo() const;
    
    bool autoUpdateTextureAxes() const;
    void setAutoUpdateTextureAxes(bool enabled);
    void autoGenerateTextureAxes();

    QPlane3D toPlane3D() const;
    
    // Reverses the order of the vertices so that the faces point in the other direction.
    void flip();

signals:
    void vertexAdded(MapVertex*);
    void vertexRemoved(MapVertex*);
    void textureInfoChanged();
    void autoUpdateTextureAxesChanged(bool);

public slots:

private slots:
    void handleItemAdded(MapVertex* vertex);
    void handleItemRemoved(MapVertex* vertex);
    void detachVertex(QObject*);
    void vertexUpdated();
    void generateTextureAxesIfRequired();

private:
    // Convenience
    void connectChangeSignal(MapVertex* vertex);
    void disconnectChangeSignal(MapVertex* vertex);
    void setupConnections();

    VertexVector* m_pVertices;
    mutable FaceTextureInfo* m_pTextureInfo;
    bool m_bUpdateTextureAxes;
    mutable bool m_bShouldUpdateTextureAxes;
};

namespace Vectors
{
    class MODELSHARED_EXPORT ExposedPVector_MapFace : public QObject
    {
        Q_OBJECT
        EXPOSED_POINTER_VECTOR_PROPS(MapFace)
    signals:
        void itemAdded(MapFace*);
        void itemRemoved(MapFace*);
    };

    class MODELSHARED_EXPORT ExposedOVector_MapFace : public QObject
    {
        Q_OBJECT
        EXPOSED_OWNER_VECTOR_PROPS(MapFace)
    signals:
        void itemAdded(MapFace*);
        void itemRemoved(MapFace*);
    };

    class MODELSHARED_EXPORT ExposedQPVector_MapFace : public QObject
    {
        Q_OBJECT
        EXPOSED_QPOINTER_VECTOR_PROPS(MapFace)
    signals:
        void itemAdded(MapFace*);
        void itemRemoved(MapFace*);
    };
}

MODEL_END_NAMESPACE

#endif // MAPFACE_H
