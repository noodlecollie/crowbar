#include "brushface.h"
#include "brushvertex.h"
#include <QVariant>

MODEL_BEGIN_NAMESPACE

BrushFace::BrushFace(QObject *parent) : QObject(parent)
{

}

void BrushFace::verticesAppend(BrushVertex *vertex)
{
    m_Vertices.append(QPointer<BrushVertex>(vertex));
}

BrushVertex* BrushFace::verticesItemAt(int index) const
{
    return m_Vertices.at(index).data();
}

int BrushFace::verticesCount() const
{
    return m_Vertices.count();
}

void BrushFace::verticesClear()
{
    m_Vertices.clear();
}

QVector3D BrushFace::normal() const
{
    if ( verticesCount() < 3 ) return QVector3D();
    
    return QVector3D::normal(verticesItemAt(0)->position(),
                             verticesItemAt(1)->position(),
                             verticesItemAt(2)->position());
}

void BrushFace::verticesClean()
{
    for ( int i = 0; i < m_Vertices.count(); )
    {
        if ( m_Vertices.at(i).isNull() )
        {
            m_Vertices.remove(i);
            continue;
        }
        
        i++;
    }
}

bool BrushFace::verticesContains(BrushVertex *vertex) const
{
    return m_Vertices.contains(QPointer<BrushVertex>(vertex));
}

void BrushFace::verticesInsertAt(int index, BrushVertex *vertex)
{
    m_Vertices.insert(index, QPointer<BrushVertex>(vertex));
}

void BrushFace::verticesRemoveAt(int index)
{
    m_Vertices.remove(index);
}

QVariant BrushFace::vertexIndices() const
{
    return QVariant::fromValue<QVector<int> >(m_VertexIndices);
}

void BrushFace::setVertexIndices(const QVariant &list)
{
    m_VertexIndices.clear();
    foreach ( QVariant item, list.toList() )
    {
        bool ok = false;
        int v = item.toInt(&ok);
        if ( ok )
        {
            m_VertexIndices.append(v);
        }
    }

    emit vertexIndicesChanged();
}

MODEL_END_NAMESPACE
