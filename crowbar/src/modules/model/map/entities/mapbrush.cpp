#include "mapbrush.h"

MODEL_BEGIN_NAMESPACE

IMPLEMENT_QML_LIST_TYPE(MapBrush, MODEL_NAMESPACE::MapVertex, vertices)
IMPLEMENT_LIST_TYPE(MapBrush, MODEL_NAMESPACE::MapVertex*, vertices)

IMPLEMENT_QML_LIST_TYPE(MapBrush, MODEL_NAMESPACE::MapEdge, edges)
IMPLEMENT_LIST_TYPE(MapBrush, MODEL_NAMESPACE::MapEdge*, edges)

IMPLEMENT_QML_LIST_TYPE(MapBrush, MODEL_NAMESPACE::MapFace, faces)
IMPLEMENT_LIST_TYPE(MapBrush, MODEL_NAMESPACE::MapFace*, faces)

MapBrush::MapBrush(QObject *parent) : MapHandleObject(parent)
{
}

MapBrush::~MapBrush()
{

}

MODEL_END_NAMESPACE
