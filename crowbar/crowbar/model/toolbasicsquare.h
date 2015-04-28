#ifndef TOOLBASICSQUARE_H
#define TOOLBASICSQUARE_H

#include "toolcomponent.h"
#include <QColor>

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT ToolBasicSquare : public ToolComponent
{
    Q_OBJECT
    MAPCOMPONENT_SUBCLASS(ToolBasicSquare, ToolComponent)

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(float borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)
    Q_PROPERTY(float size READ size WRITE setSize NOTIFY sizeChanged)
public:

    QColor color() const;
    void setColor(const QColor &col);

    QColor borderColor() const;
    void setBorderColor(const QColor &col);

    float borderWidth() const;
    void setBorderWidth(float width);

    float size() const;
    void setSize(float size);

signals:
    void colorChanged(const QColor&);
    void borderColorChanged(const QColor&);
    void borderWidthChanged(float);
    void sizeChanged(float);

public slots:

private:
    QColor  m_Colour;
    QColor  m_BorderColour;
    float   m_flBorderWidth;
    float   m_flSize;
};

MODEL_END_NAMESPACE

#endif // TOOLBASICSQUARE_H
