#include "toolsetitem.h"
#include "basetool.h"
#include <QDockWidget>

TOOLS_BEGIN_NAMESPACE

ToolSetItem::ToolSetItem(QObject *parent) :
    QObject(parent)
{
}

ToolSetItem::~ToolSetItem()
{
}

BaseTool* ToolSetItem::tool() const
{
    return m_pTool.data();
}

QDockWidget* ToolSetItem::toolDockWidget() const
{
    return m_pDockWidget.data();
}

void ToolSetItem::setTool(BaseTool *tool)
{
    if ( tool == m_pTool.data() ) return;
    
    m_pTool = tool;
}

void ToolSetItem::setToolDockWidget(QDockWidget *widget)
{
    if ( widget == m_pDockWidget.data() ) return;
    
    m_pDockWidget = widget;
}

TOOLS_END_NAMESPACE
