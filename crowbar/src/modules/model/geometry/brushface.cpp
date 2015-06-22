#include "brushface.h"
#include "brushvertex.h"

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

MODEL_END_NAMESPACE
