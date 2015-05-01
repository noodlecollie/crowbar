#ifndef LEAFCLASS_H
#define LEAFCLASS_H

#include "subclass.h"

class Leafclass : public Subclass
{
    Q_OBJECT
    Q_PROPERTY(float floatProperty READ floatProperty WRITE setFloatProperty NOTIFY floatPropertyChanged)
public:
    Leafclass(QObject* parent = 0);
    ~Leafclass();
    
    float floatProperty() const { return m_flFloatProperty; }
    void setFloatProperty(float value) { m_flFloatProperty = value; emit floatPropertyChanged(m_flFloatProperty); }
    
signals:
    void floatPropertyChanged(float);
    
private:
    float m_flFloatProperty;
};

#endif // LEAFCLASS_H
