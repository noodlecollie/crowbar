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

MODEL_END_NAMESPACE
