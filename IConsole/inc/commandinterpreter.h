#ifndef COMMANDINTERPRETER_H
#define COMMANDINTERPRETER_H

#include "iconsole_global.h"
#include "nglobalcmd.h"
#include <QObject>
#include <QString>
#include <QList>
#include <QPair>

class CommandManager;

class ICONSOLESHARED_EXPORT CommandInterpreter : public QObject
{
    Q_OBJECT
public:
    typedef QPair<QString, QStringList> CommandEntryPair;
    typedef QList<CommandEntryPair> CommandEntryPipeList;
    typedef QList<CommandEntryPipeList> CommandEntryList;
    
    typedef QPair<NGlobalCmd::CmdIdent, QString> CommandIdentPair;
    
    explicit CommandInterpreter(CommandManager* manager, QObject *parent = 0);
    explicit CommandInterpreter(QObject *parent = 0);
    
    void setManager(CommandManager* manager);
    CommandManager* getManager() const;
    
    // Parses command string - separates command name and arguments, and determines command pipes.
    // When piping, the output variant is converted to a QStringList to be used as the arguments for the next command.
    void parse(const QString &cmdString);
    
    // Suggestions are returned as a pair containing the ident and the command name.
    void getSuggestions(const QString &prefix, QList<CommandIdentPair> &list);
    
signals:
    
public slots:
    
private:
    void parseCommandString(const QString &cmdString, CommandEntryList &masterList);
    
    CommandManager*     m_pCommandManager;
};

#endif // COMMANDINTERPRETER_H
