#include "maplightbase.h"
#include <QGLLightParameters>

// Change this to change the brightness of the light as seen in the editor.
// Smaller values mean less brightness.
#define BRIGHTNESS_CONSTANT 1.0f

MODEL_BEGIN_NAMESPACE

void MapLightBase::initialise(MapComponent *parent)
{
    setParentComponent(parent);
    
    m_pParams = new QGLLightParameters(this);
    connect(m_pParams, SIGNAL(lightChanged()), this, SLOT(sendRenderDataChange()));
}

void MapLightBase::dispose()
{
}

QBox3D MapLightBase::localBoundingBox() const
{
    return QBox3D(QVector3D(-8,-8,-8), QVector3D(8,8,8));
}

QVector3D MapLightBase::localCentroid() const
{
    return QVector3D(0,0,0);
}

QGLLightParameters* MapLightBase::parameters()
{
    return m_pParams;
}

const QGLLightParameters* MapLightBase::parameters() const
{
    return m_pParams;
}

QVector4D MapLightBase::standardLightColor() const
{
    QColor col = m_pParams->diffuseColor();
    return QVector4D(col.red(), col.green(), col.blue(), brightness());
}

int MapLightBase::brightness() const
{
    return (int)((1.0f/BRIGHTNESS_CONSTANT) * (1.0f/m_pParams->constantAttenuation()));
}

void MapLightBase::setBrightness(int value)
{
    float val = (1.0f/(float)value) / (1.0f/BRIGHTNESS_CONSTANT);
    m_pParams->setConstantAttenuation(val);
}

MODEL_END_NAMESPACE
