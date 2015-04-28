#include "toolbasicsquare.h"

MODEL_BEGIN_NAMESPACE

void ToolBasicSquare::initialise(MapComponent *parent)
{
    setParentComponent(parent);
    m_flBorderWidth = 1.0f;
    m_Colour = QColor(255,255,255);
    m_BorderColour = QColor(0,0,0);
    m_flSize = 1.0f;
    setBillboard(true);
}

void ToolBasicSquare::dispose()
{
}

QColor ToolBasicSquare::color() const
{
    return m_Colour;
}

void ToolBasicSquare::setColor(const QColor &col)
{
    if ( m_Colour == col ) return;

    m_Colour = col;
    emit colorChanged(m_Colour);
    sendRenderDataChange();
}

QColor ToolBasicSquare::borderColor() const
{
    return m_BorderColour;
}

void ToolBasicSquare::setBorderColor(const QColor &col)
{
    if ( col == m_BorderColour ) return;

    m_BorderColour = col;
    emit borderColorChanged(m_BorderColour);
    sendRenderDataChange();
}

float ToolBasicSquare::borderWidth() const
{
    return m_flBorderWidth;
}

void ToolBasicSquare::setBorderWidth(float width)
{
    if ( m_flBorderWidth == width ) return;

    m_flBorderWidth = width;
    emit borderWidthChanged(m_flBorderWidth);
    sendRenderDataChange();
}

float ToolBasicSquare::size() const
{
    return m_flSize;
}

void ToolBasicSquare::setSize(float size)
{
    if ( size == m_flSize ) return;

    m_flSize = size;
    emit sizeChanged(m_flSize);
    sendRenderDataChange();
}

MODEL_END_NAMESPACE
