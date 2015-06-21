#include "brush.h"
#include <QVector3D>
#include <QVector>
#include "brushvertexreference.h"

MODEL_BEGIN_NAMESPACE

struct VertexPrivate
{
    QVector3D position;
    QSet<FacePrivate*> linkedFaces;
    QVector<BrushVertexReference*> references;
};

struct FacePrivate
{
    QVector<VertexPrivate*> linkedVertices;
};

Brush::Brush(QObject *parent) : ChangeNotifier(parent)
{
}

VertexPrivate* Brush::createVertex()
{
    VertexPrivate* v = new VertexPrivate;
    m_Vertices.insert(v);
    return v;
}

void Brush::destroyVertex(VertexPrivate *v)
{
    // Remove this vertex from any faces in which it is referenced.
    foreach ( FacePrivate* f, v->linkedFaces )
    {
        f->linkedVertices.removeAll(v);
    }

    m_Vertices.remove(v);
    delete v;
}

FacePrivate* Brush::createFace()
{
    FacePrivate* f = new FacePrivate;
    m_Faces.insert(f);
    return f;
}

void Brush::destroyFace(FacePrivate *f)
{
    // Remove this face from any vertices in which it is referenced.
    foreach ( VertexPrivate* v, f->linkedVertices )
    {
        v->linkedFaces.remove(f);
    }

    m_Faces.remove(f);
    delete f;
}

MODEL_END_NAMESPACE
