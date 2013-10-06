#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include "iconsole_global.h"
#include <QObject>
#include <QString>
#include <QRegularExpression>
#include "nglobalcmd.h"

class BaseConsoleCommand;
class ConCommand;
class ConVar;

class ICONSOLESHARED_EXPORT CommandManager : public QObject
{
    Q_OBJECT
public:
    typedef QMap<QString, BaseConsoleCommand*> BaseCommandMap;
    
    explicit CommandManager(QObject* parent = 0);
    virtual ~CommandManager() {}
    
    bool registerCommand(BaseConsoleCommand *command);
    void unregisterCommand(const QString &name);
    bool isRegistered(const QString &name) const;
    BaseConsoleCommand* get(const QString &name) const;
    void findRegex(const QRegularExpression &regex, QList<BaseConsoleCommand *> &outList, int count = -1) const;
    void findRegex(const QString &regex, QList<BaseConsoleCommand *> &outList, int count = -1) const;
    void findPrefix(const QString &prefix, QList<BaseConsoleCommand *> &outList, int count = -1) const;
    
    ConCommand* getCommand(const QString &name) const;
    ConVar* getVariable(const QString &name) const;
    NGlobalCmd::CmdIdent exec(const QString &name, const QStringList &args, int &retVal, QVariant &output);
    
    BaseCommandMap::const_iterator constBegin();
    BaseCommandMap::const_iterator constEnd();
    
signals:
    void printMessage(QString &output);
    void printWarning(QString &output);
    
public slots:
    
private:
    BaseCommandMap     m_CommandMap;
};

#endif // COMMANDMANAGER_H
