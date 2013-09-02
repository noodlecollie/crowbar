#ifndef ICOMPLETIONWIDGET_H
#define ICOMPLETIONWIDGET_H

#include "iconsole_global.h"
#include <QString>

class ICONSOLESHARED_EXPORT ICompletionWidget
{
public:
    virtual ~ICommandManager() {}
    
    // Show list and highlight first item.
    virtual void showList(QStringList &list) = 0;
    virtual QString getHighlighted() const = 0;
    virtual void incrementHighlight() = 0;
};

//! \def ICompletionWidget_iid
//! \brief Unique ID for ICompletionWidget interface.
#define ICompletionWidget_iid "Crowbar.Interfaces.IConsole.ICompletionWidget"
Q_DECLARE_INTERFACE(ICompletionWidget, ICompletionWidget_iid)

#endif // ICOMPLETIONWIDGET_H
