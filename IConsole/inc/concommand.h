#ifndef CONCOMMAND_H
#define CONCOMMAND_H

#include "iconsole_global.h"
#include <QString>
#include <QList>
#include <QVariant>
#include "nglobalcmd.h"
#include "listedconsolecommand.h"

class CommandManager;

class ICONSOLESHARED_EXPORT ConCommand : public ListedConsoleCommand
{
public:
    explicit ConCommand(const QString &name, NGlobalCmd::CmdCallback callback, const QString &desc = "",
                            NGlobalCmd::CMDFLAGS flags = 0, QObject *parent = 0);
    
    explicit ConCommand(const QString &name, NGlobalCmd::CmdCallback callback, CommandManager* manager,
                            ListedConsoleCommand** list, const QString &desc = "", const NGlobalCmd::CMDFLAGS flags = 0, QObject* parent = 0);
    
    virtual ~ConCommand() {}
    
    virtual NGlobalCmd::CmdIdent identify() const;
    int exec(const QStringList &args, QVariant &output);
    NGlobalCmd::CmdCallback getExec() const;
    void setExec(NGlobalCmd::CmdCallback cmd);
    
signals:
    
public slots:
    
private:
    NGlobalCmd::CmdCallback    m_pCallback;
};

#endif // CONCOMMAND_H
