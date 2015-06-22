#include "brush.h"
#include "brushvertex.h"
#include "brushface.h"
#include <QSet>

MODEL_BEGIN_NAMESPACE

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

MODEL_END_NAMESPACE
