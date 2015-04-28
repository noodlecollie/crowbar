#ifndef TOOLWIREFRAMEBOX_H
#define TOOLWIREFRAMEBOX_H

#include "toolcomponent.h"
#include <QBox3D>
#include <QColor>

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT ToolWireframeBox : public ToolComponent
{
    Q_OBJECT
    MAPCOMPONENT_SUBCLASS(ToolWireframeBox, ToolComponent)

    Q_PROPERTY(QBox3D box READ box WRITE setBox NOTIFY boxChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
public:
    QBox3D box() const;
    void setBox(const QBox3D &b);

    QColor color() const;
    void setColor(const QColor &col);

signals:
    void boxChanged(const QBox3D&);
    void colorChanged(const QColor&);

private:
    QBox3D  m_Box;
    QColor  m_colColour;
};

MODEL_END_NAMESPACE

#endif // TOOLWIREFRAMEBOX_H
