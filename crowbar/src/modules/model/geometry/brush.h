#ifndef BRUSH_H
#define BRUSH_H

#include "model_global.h"
#include <Qt3DCore/QEntity>
#include <QVector>
#include <QQmlListProperty>

MODEL_BEGIN_NAMESPACE

class BrushVertex;
class BrushFace;
class BrushEdge;

class MODELSHARED_EXPORT Brush : public Qt3D::QEntity
{
    Q_OBJECT

    // ALWAYS fully qualify namespaces for these macros.
    Q_PROPERTY(QQmlListProperty<MODEL_NAMESPACE::BrushVertex> vertices READ vertices)
    Q_PROPERTY(QQmlListProperty<MODEL_NAMESPACE::BrushFace> faces READ faces)
public:
    explicit Brush(QNode *parent = 0);

    void verticesAppend(BrushVertex* vertex);
    BrushVertex* verticesItemAt(int index) const;
    int verticesCount() const;
    void verticesClear();
    void verticesClean();
    QQmlListProperty<BrushVertex> vertices();

    bool verticesContains(BrushVertex* vertex) const;
    void verticesInsertAt(int index, BrushVertex* vertex);
    void verticesRemoveAt(int index);

    void facesAppend(BrushFace* face);
    BrushFace* facesItemAt(int index) const;
    int facesCount() const;
    void facesClear();
    void facesClean();
    int totalFaceVertices() const;
    QQmlListProperty<BrushFace> faces();

    bool facesContains(BrushFace* face) const;
    void facesInsertAt(int index, BrushFace* face);
    void facesRemoveAt(int index);

    void edgesAppend(BrushEdge* edge);
    BrushEdge* edgesItemAt(int index) const;
    int edgesCount() const;
    void edgesClear();

    // Replaces the vertex pointer list of each face with pointers
    // to the vertices specified in the index array of the face.
    void convertFaceVertexIndices();

    // Populate the edge list with the edges from the given faces.
    void populateEdgesFromFaces();

signals:

public slots:

private:
    QVector<BrushVertex*> m_Vertices;
    QVector<BrushFace*> m_Faces;
    QVector<BrushEdge*> m_Edges;
};

MODEL_END_NAMESPACE

#endif // BRUSH_H
