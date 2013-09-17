#ifndef BASECONSOLECOMMAND_H
#define BASECONSOLECOMMAND_H

#include "iconsole_global.h"
#include <QObject>
#include <QList>
#include <QString>
#include "nglobalcmd.h"

class ICONSOLESHARED_EXPORT BaseConsoleCommand : public QObject
{
    Q_OBJECT
public:
    // Constructor
    explicit BaseConsoleCommand(const QString &name, const QString &desc = "", NGlobalCmd::CMDFLAGS flags = 0, QObject* parent = 0);
    virtual ~BaseConsoleCommand() {}
    
    NGlobalCmd::CmdIdent identify() const;
    const QString& getName() const;
    const QString& getDescription() const;
    NGlobalCmd::CMDFLAGS getFlags() const;
    void setFlags(NGlobalCmd::CMDFLAGS flags);
    
signals:
    
public slots:
    
private:
    QString                 m_szName;
    QString                 m_szDescription;
    NGlobalCmd::CMDFLAGS    m_iFlags;
};

#endif // BASECONSOLECOMMAND_H
