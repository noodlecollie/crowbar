#ifndef COMMANDSENDERINFO_H
#define COMMANDSENDERINFO_H

#include <QObject>

class QString;
class CommandManager;

class CommandSenderInfo : public QObject
{
    Q_OBJECT
public:
    enum OutputType
    {
        OutputGeneral = 0,
        OutputWarning
    };
    
    typedef void (CommandManager::*SendOutput) (CommandSenderInfo::OutputType, const QString &);
    
    explicit CommandSenderInfo(QString name, CommandManager* manager, SendOutput outputFunc, QObject *parent = 0);
    
    void writeOutput(CommandSenderInfo::OutputType type, const QString &output) const;
    
    CommandSenderInfo::SendOutput getOutputPointer() const;
    void setOutputPointer(CommandSenderInfo::SendOutput output);
    
    QString getName() const;
    void setName(QString &name);
    
    const CommandManager* getManager() const;
    void setManager(CommandManager* manager);
    
signals:
    
public slots:
    
private:
    CommandManager* m_pManager;
    SendOutput      m_pOutput;
    QString         m_NameOfCommand;
};

#endif // COMMANDSENDERINFO_H
