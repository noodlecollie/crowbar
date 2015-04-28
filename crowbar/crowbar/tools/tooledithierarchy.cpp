#include "tooledithierarchy.h"

TOOLS_BEGIN_NAMESPACE

ToolEditHierarchy::ToolEditHierarchy(QObject *parent) :
    BaseTool(parent)
{
}

ToolEditHierarchy::~ToolEditHierarchy()
{
}

QString ToolEditHierarchy::toolName() const
{
    return "Hierarchy";
}

TOOLS_END_NAMESPACE
