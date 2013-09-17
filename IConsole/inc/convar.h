#ifndef CONVAR_H
#define CONVAR_H

#include "iconsole_global.h"
#include <QString>
#include <QVariant>
#include "nglobalcmd.h"
#include "listedconsolecommand.h"

class CommandManager;

class ConVar : public ListedConsoleCommand
{
public:
    explicit ConVar(const QString &name, const QString &def, NGlobalCmd::VarCallback callback = NULL, const QString &desc = "",
                    NGlobalCmd::CMDFLAGS flags = 0, bool hasMin = false, float min = 0.0, bool hasMax = false, float max = 0.0, QObject *parent = 0);
    
    explicit ConVar(const QString &name, const QString &def, CommandManager* manager, ListedConsoleCommand* list,
                    NGlobalCmd::VarCallback callback = NULL, const QString &desc = "", NGlobalCmd::CMDFLAGS flags = 0,
                    bool hasMin = false, float min = 0.0f, bool hasMax = false, float max = 0.0f, QObject* parent = 0);
    
    virtual ~ConVar() {}
    
    void setCallback(NGlobalCmd::VarCallback callback);
    NGlobalCmd::VarCallback getCallback() const;
    QString get() const;
    QString set(const QString &value);
    bool hasMin() const;
    float getMin() const;
    void setMin(float value);
    bool hasMax() const;
    float getMax() const;
    void setMax(float value);
    QString getDefault() const;
    void setToDefault();
    
    QString getConVarString() const;
    QString setConVar(const QString &val);
    int getConVarInt() const;
    int setConVar(int val);
    float getConVarFloat() const;
    float setConVar(float val);
    bool getConVarBool() const;
    bool setConVar(bool val);
    
private:
    void validateBounds(float &min, float &max);
    
    NGlobalCmd::VarCallback     m_pVarCallback;
    QVariant                    m_Variable;
    QVariant                    m_Default;
    bool                        m_bHasMin;
    bool                        m_bHasMax;
    float                       m_flMinVal;
    float                       m_flMaxVal;
};

#endif // CONVAR_H
