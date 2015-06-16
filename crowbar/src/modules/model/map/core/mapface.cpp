#include "mapface.h"

MODEL_BEGIN_NAMESPACE

IMPLEMENT_LIST_TYPE(MapFace, int, vertices)
IMPLEMENT_LIST_TYPE(MapFace, int, edges)

MapFace::MapFace(QObject *parent) : MapHandleObject(parent)
{
}

MapFace::~MapFace()
{
}

QVector3D MapFace::normal() const
{
    // TODO
    return QVector3D();
}

bool MapFace::isValid() const
{
    // TODO
    return false;
}

QVariant MapFace::vertices() const
{
    return QVariant::fromValue<QVector<int> >(m_verticesList);
}

void MapFace::setVertices(const QVariant &list)
{
    verticesClear();
    foreach ( QVariant item, list.toList() )
    {
        bool ok = false;
        int v = item.toInt(&ok);
        if ( ok )
        {
            verticesAppend(v);
        }
    }

    emit verticesChanged();
}

QVariant MapFace::edges() const
{
    return QVariant::fromValue<QVector<int> >(m_edgesList);
}

void MapFace::setEdges(const QVariant &list)
{
    edgesClear();
    foreach ( QVariant item, list.toList() )
    {
        bool ok = false;
        int e = item.toInt(&ok);
        if ( ok )
        {
            edgesAppend(e);
        }
    }

    emit edgesChanged();
}

MODEL_END_NAMESPACE
