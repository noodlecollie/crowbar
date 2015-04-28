#ifndef MAPLIGHT_H
#define MAPLIGHT_H

#include "model_global.h"
#include "mapcomponent.h"
#include <QVector4D>

class QGLLightParameters;

MODEL_BEGIN_NAMESPACE

// Base component object for lights.
class MODELSHARED_EXPORT MapLightBase : public MapComponent
{
    Q_OBJECT
    MAPCOMPONENT_SUBCLASS(MapLightBase, MapComponent)
public:
    virtual QBox3D localBoundingBox() const;
    virtual QVector3D localCentroid() const;
    
    QVector4D standardLightColor() const;
    void setstandardLightColor(const QVector4D &col);
    void setStandardLightColor(const QColor &col, int brightness);
    void setstandardLightColor(int red, int green, int blue, int brightness);
    
    int brightness() const;
    void setBrightness(int value);
    
    const QGLLightParameters* parameters() const;
    
protected:
    QGLLightParameters* parameters();
    
signals:
    
public slots:
    
private slots:
    
private:
    QGLLightParameters* m_pParams;
};

MODEL_END_NAMESPACE

#endif // MAPLIGHT_H
