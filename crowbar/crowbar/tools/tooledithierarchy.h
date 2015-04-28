#ifndef TOOLEDITHIERARCHY_H
#define TOOLEDITHIERARCHY_H

#include "basetool.h"

TOOLS_BEGIN_NAMESPACE

class TOOLSSHARED_EXPORT ToolEditHierarchy : public BaseTool
{
    Q_OBJECT
public:
    ToolEditHierarchy(QObject* parent = 0);
    virtual ~ToolEditHierarchy();
    
    virtual QString toolName() const;
};

TOOLS_END_NAMESPACE

#endif // TOOLEDITHIERARCHY_H
