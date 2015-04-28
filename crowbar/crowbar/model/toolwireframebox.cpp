#include "toolwireframebox.h"

MODEL_BEGIN_NAMESPACE

void ToolWireframeBox::initialise(MapComponent *parent)
{
    setParentComponent(parent);
    m_colColour = QColor(255,255,255);
}

void ToolWireframeBox::dispose()
{
}

QBox3D ToolWireframeBox::box() const
{
    return m_Box;
}

void ToolWireframeBox::setBox(const QBox3D &b)
{
    if ( m_Box == b ) return;

    m_Box = b;
    emit boxChanged(m_Box);
    sendRenderDataChange();
}

QColor ToolWireframeBox::color() const
{
    return m_colColour;
}

void ToolWireframeBox::setColor(const QColor &col)
{
    if ( m_colColour == col ) return;

    m_colColour = col;
    emit colorChanged(m_colColour);
    sendRenderDataChange();
}

MODEL_END_NAMESPACE
