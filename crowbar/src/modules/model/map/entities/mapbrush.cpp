#include "mapbrush.h"

MODEL_BEGIN_NAMESPACE

IMPLEMENT_QML_LIST_TYPE(MapBrush, MapVertex, vertices)
IMPLEMENT_LIST_TYPE(MapBrush, MapVertex*, vertices)

IMPLEMENT_QML_LIST_TYPE(MapBrush, MapEdge, edges)
IMPLEMENT_LIST_TYPE(MapBrush, MapEdge*, edges)

IMPLEMENT_QML_LIST_TYPE(MapBrush, MapFace, faces)
IMPLEMENT_LIST_TYPE(MapBrush, MapFace*, faces)

MapBrush::MapBrush(QObject *parent) : MapHandleObject(parent)
{
}

MapBrush::~MapBrush()
{

}

MODEL_END_NAMESPACE
