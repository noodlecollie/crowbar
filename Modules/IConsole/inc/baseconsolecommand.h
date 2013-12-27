/** \file baseconsolecommand.h
 * \brief Describes the base properties common to all console commands and variables.
 */

/**
 * \defgroup IConsole IConsole library
 * @{
 */

#ifndef BASECONSOLECOMMAND_H
#define BASECONSOLECOMMAND_H

#include "iconsole_global.h"
#include <QObject>
#include <QList>
#include <QString>
#include "nglobalcmd.h"

/**
 * @brief Class defining properties common to all console commands.
 *
 * A BaseConsoleCommand contains data referring to the command's name, description, flags and identification.
 * A BaseConsoleCommand identifies as NGlobalCommand::CmdIdent::CINull.
 */
class ICONSOLESHARED_EXPORT BaseConsoleCommand : public QObject
{
    Q_OBJECT
    /** @property name
     * @brief Name of the command.
     * @accessors name()
     */
    Q_PROPERTY(QString name READ name)
    
    /** @property description
     * @brief Description of the command.
     * @accessors description()
     */
    Q_PROPERTY(QString description READ description)
public:
    /**
     * @brief Constructor
     * @param name Name of the command.
     * @param desc Optional description of the command.
     * @param flags Command flags.
     * @param parent Parent QObject, if applicable.
     */
    explicit BaseConsoleCommand(const QString &name, const QString &desc = "", NGlobalCmd::CMDFLAGS flags = 0, QObject* parent = 0);
    
    /**
     * @brief Destructor.
     */
    virtual ~BaseConsoleCommand() {}
    
    /**
     * @brief Identifies this console command. A BaseConsoleCommand object will return CINull; other derived
     * commands should return a specific identifier.
     * @return Identifier for the console command.
     */
    virtual NGlobalCmd::CmdIdent identify() const;
    
    /**
     * @brief Returns the name of the command.
     * @return Command name.
     */
    QString name() const;
    
    /**
     * @brief Returns the description of the command.
     * @return Command description, or empty string if no description was set.
     */
    QString description() const;
    
    /**
     * @brief Returns the command's collection of flags.
     * @return Flags.
     */
    NGlobalCmd::CMDFLAGS flagsRaw() const;
    
    /**
     * @brief Overwrites all flags on the command.
     * @param flags Integer containing the exact state of all flags to set.
     */
    virtual void setFlagsRaw(NGlobalCmd::CMDFLAGS flags);
    
    /**
     * @brief Returns whether a given flag is set. Can test combinations of flags.
     * @param flag Flag(s) to test for.
     * @return True if flag is set (or in the case of multiple test flags, if all are set), false otherwise.
     */
    bool flagSet(NGlobalCmd::CMDFLAGS flag) const;
    
    /**
     * @brief Sets the given flag(s) on the command.
     * @param flag Flag(s) to set.
     */
    virtual void setFlag(NGlobalCmd::CMDFLAGS flag);
    
    /**
     * @brief Un-sets the given flag(s) from the command.
     * @param flag Flag(s) to Un-set.
     */
    void removeFlag(NGlobalCmd::CMDFLAGS flag);
    
    /**
     * @brief Toggles the given flag(s) on the command.
     * @param flag Flag(s) to toggle.
     */
    virtual void toggleFlag(NGlobalCmd::CMDFLAGS flag);
    
signals:
    
public slots:
    
private:
    QString                 m_szName;           /**< Name of command. */
    QString                 m_szDescription;    /**< Description of command. */
    NGlobalCmd::CMDFLAGS    m_iFlags;           /**< Command flags. */
};

#endif // BASECONSOLECOMMAND_H

/**@}*/
