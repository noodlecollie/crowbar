#include "positionalsubcomponent.h"

MODEL_BEGIN_NAMESPACE

PositionalSubComponent::PositionalSubComponent(MapComponent *parent) :
    MapSubComponent(parent)
{
}

QBox3D PositionalSubComponent::boundingBox() const
{
    return QBox3D();
}

QVector3D PositionalSubComponent::centroid() const
{
    return QVector3D();
}

bool PositionalSubComponent::isValid() const
{
    return true;
}

MODEL_END_NAMESPACE
