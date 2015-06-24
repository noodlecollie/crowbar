#include "brush.h"
#include "brushvertex.h"
#include "brushface.h"
#include <QSet>

MODEL_BEGIN_NAMESPACE

// ///////////////////////////////////////
// Begin QQmlListProperty helper functions
// ///////////////////////////////////////

static void qmlVerticesAppend(QQmlListProperty<BrushVertex> *property, BrushVertex *value)
{
    static_cast<Brush*>(property->object)->verticesAppend(value);
}

static BrushVertex* qmlVerticesItemAt(QQmlListProperty<BrushVertex> *property, int index)
{
    return static_cast<Brush*>(property->object)->verticesItemAt(index);
}

static void qmlVerticesClear(QQmlListProperty<BrushVertex> *property)
{
    static_cast<Brush*>(property->object)->verticesClear();
}

static int qmlVerticesCount(QQmlListProperty<BrushVertex> *property)
{
    return static_cast<Brush*>(property->object)->verticesCount();
}

static void qmlFacesAppend(QQmlListProperty<BrushFace> *property, BrushFace *value)
{
    static_cast<Brush*>(property->object)->facesAppend(value);
}

static BrushFace* qmlFacesItemAt(QQmlListProperty<BrushFace> *property, int index)
{
    return static_cast<Brush*>(property->object)->facesItemAt(index);
}

static void qmlFacesClear(QQmlListProperty<BrushFace> *property)
{
    static_cast<Brush*>(property->object)->facesClear();
}

static int qmlFacesCount(QQmlListProperty<BrushFace> *property)
{
    return static_cast<Brush*>(property->object)->facesCount();
}

// ///////////////////////////////////////
//  End QQmlListProperty helper functions
// ///////////////////////////////////////

Brush::Brush(QNode *parent) : Qt3D::QEntity(parent)
{
}

void Brush::verticesAppend(BrushVertex *vertex)
{
    m_Vertices.append(vertex);
}

BrushVertex* Brush::verticesItemAt(int index) const
{
    return m_Vertices.at(index);
}

int Brush::verticesCount() const
{
    return m_Vertices.count();
}

void Brush::verticesClear()
{
    qDeleteAll(m_Vertices);
    m_Vertices.clear();
}

void Brush::facesAppend(BrushFace *face)
{
    m_Faces.append(face);
}

BrushFace* Brush::facesItemAt(int index) const
{
    return m_Faces.at(index);
}

int Brush::facesCount() const
{
    return m_Faces.count();
}

void Brush::facesClear()
{
    qDeleteAll(m_Faces);
    m_Faces.clear();
}

void Brush::verticesClean()
{
    QSet<BrushVertex*> verts;
    for ( int i = 0; i < verticesCount(); i++ )
    {
        verts.insert(verticesItemAt(i));
    }

    foreach ( BrushFace* f, m_Faces )
    {
        for ( int i = 0; i < f->verticesCount(); i++ )
        {
            verts.remove(f->verticesItemAt(i));
        }
    }

    foreach ( BrushVertex* v, verts )
    {
        m_Vertices.removeOne(v);    // These should all be unique.
        delete v;
    }
}

void Brush::facesClean()
{
    for ( int i = 0; i < facesCount(); )
    {
        BrushFace* f = facesItemAt(i);
        if ( f->verticesCount() < 1 )
        {
            m_Faces.remove(i);
            delete f;
            continue;
        }

        i++;
    }
}

int Brush::totalFaceVertices() const
{
    int verts = 0;
    foreach ( BrushFace* f, m_Faces )
    {
        verts += f->verticesCount();
    }
    
    return verts;
}

bool Brush::verticesContains(BrushVertex *vertex) const
{
    return m_Vertices.contains(vertex);
}

bool Brush::facesContains(BrushFace *face) const
{
    return m_Faces.contains(face);
}

void Brush::verticesInsertAt(int index, BrushVertex *vertex)
{
    m_Vertices.insert(index, vertex);
}

void Brush::facesInsertAt(int index, BrushFace *face)
{
    m_Faces.insert(index, face);
}

void Brush::verticesRemoveAt(int index)
{
    m_Vertices.removeAt(index);
}

void Brush::facesRemoveAt(int index)
{
    m_Faces.removeAt(index);
}

QQmlListProperty<BrushVertex> Brush::vertices()
{
    return QQmlListProperty<BrushVertex>(this, NULL,
                                         &qmlVerticesAppend,
                                         &qmlVerticesCount,
                                         &qmlVerticesItemAt,
                                         &qmlVerticesClear);
}

QQmlListProperty<BrushFace> Brush::faces()
{
    return QQmlListProperty<BrushFace>(this, NULL,
                                         &qmlFacesAppend,
                                         &qmlFacesCount,
                                         &qmlFacesItemAt,
                                         &qmlFacesClear);
}

void Brush::convertFaceVertexIndices()
{
    foreach ( BrushFace* f, m_Faces )
    {
        f->verticesClear();

        QVector<int> indices = f->vertexIndices().value<QVector<int> >();
        foreach ( int i, indices )
        {
            if ( i < 0 || i >= verticesCount() ) continue;

            f->verticesAppend(verticesItemAt(i));
        }
    }
}

MODEL_END_NAMESPACE
