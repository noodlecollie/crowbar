#include "maplightpositional.h"
#include <QGLLightParameters>

MODEL_BEGIN_NAMESPACE

void MapLightPositional::initialise(MapComponent *parent)
{
    setParentComponent(parent);
    
    connect(hierarchyTransform(), SIGNAL(propertiesChanged()), this, SLOT(handleTransformChanged()));
    connect(hierarchyTransform(), SIGNAL(parentPropertiesChanged(HierarchyTransform_New*)), this, SLOT(handleTransformChanged()));
}

void MapLightPositional::dispose()
{
}

void MapLightPositional::handleTransformChanged()
{
    updateLightPosition();
}

void MapLightPositional::updateLightPosition()
{
    QVector4D pos = hierarchyTransform()->globalTransformMatrix().column(3);
    parameters()->setPosition(QVector3D(pos.x(), pos.y(), pos.z()));
}

MODEL_END_NAMESPACE
