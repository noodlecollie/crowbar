#ifndef LISTEDCONSOLECOMMAND_H
#define LISTEDCONSOLECOMMAND_H

#include "iconsole_global.h"
#include <QString>
#include "nglobalcmd.h"
#include "baseconsolecommand.h"

class CommandManager;

class ICONSOLESHARED_EXPORT ListedConsoleCommand : public BaseConsoleCommand
{
public:
    explicit ListedConsoleCommand(const QString &name, CommandManager* manager, ListedConsoleCommand* listHead,
                                  const QString &desc = "", NGlobalCmd::CMDFLAGS flags = 0, QObject *parent = 0);
    
    explicit ListedConsoleCommand(const QString &name, const QString &desc = "", NGlobalCmd::CMDFLAGS flags = 0, QObject *parent = 0);
    virtual ~ListedConsoleCommand() {}
    
    ListedConsoleCommand* getListNext() const;
    void setListNext(ListedConsoleCommand *next);
    void tryRegister(CommandManager *manager, ListedConsoleCommand *listHead);
    
signals:
    
public slots:
    
private:
    ListedConsoleCommand*     m_pListNext;
};

#endif // LISTEDCONSOLECOMMAND_H
