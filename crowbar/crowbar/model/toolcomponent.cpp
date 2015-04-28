#include "toolcomponent.h"

MODEL_BEGIN_NAMESPACE

void ToolComponent::initialise(MapComponent *parent)
{
    setParentComponent(parent);
    m_iVisibleRenderModes = ToolComponent::Triangles | ToolComponent::Edges;
}

void ToolComponent::dispose()
{
}

QString ToolComponent::ownerTool() const
{
    return m_szOwnerTool;
}

void ToolComponent::setOwnerTool(const QString &toolName)
{
    if ( toolName == m_szOwnerTool ) return;

    m_szOwnerTool = toolName;
    emit ownerToolChanged(m_szOwnerTool);
}

int ToolComponent::visibleRenderModes() const
{
    return m_iVisibleRenderModes;
}

void ToolComponent::setVisibleRenderModes(int modes)
{
    if ( modes == m_iVisibleRenderModes ) return;

    m_iVisibleRenderModes = modes;
    emit visibleRenderModesChanged(m_iVisibleRenderModes);
    sendRenderDataChange();
}

MODEL_END_NAMESPACE
