/** \file commandsenderinfo.h
 */

/**
 * \defgroup IConsole IConsole library
 * @{
 */

#ifndef COMMANDSENDERINFO_H
#define COMMANDSENDERINFO_H

#include "iconsole_global.h"
#include <QObject>

class QString;
class CommandManager;

/**
 * @brief The CommandSenderInfo class provides a route between a calling CommandManager and a receiving ConCommand or ConVar.
 * The command or variable can print output to the relevant console window (if one exists) by passing the output to this class.
 */
class ICONSOLESHARED_EXPORT CommandSenderInfo : public QObject
{
    Q_OBJECT
    
    /** @property outputCallback
     * @brief Callback to send output through.
     * @accessors outputCallback(), setOutputCallback()
     */
    Q_PROPERTY(SendOutput outputCallback READ outputCallback WRITE setOutputCallback)
    
    /** @property commandName
     * @brief Name of command this object is assigned to.
     * @accessors commandName(), setCommandName()
     */
    Q_PROPERTY(QString commandName READ commandName WRITE setCommandName)
    
    
    /** @property hasMin
     * @brief Whether the variable has a min value. Only used for ConVars.
     * @accessors hasMin(), setHasMin()
     */
    Q_PROPERTY(bool hasMin READ hasMin WRITE setHasMin)
    
    /** @property hasMax
     * @brief Whether the variable has a max value. Only used for ConVars.
     * @accessors hasMax(), setHasMax()
     */
    Q_PROPERTY(bool hasMax READ hasMax WRITE setHasMax)
    
    /** @property minValue
     * @brief Variable's min value. Only used for ConVars.
     * @accessors minValue(), setMinValue()
     */
    Q_PROPERTY(float minValue READ minValue WRITE setMinValue)
    
    /** @property maxValue
     * @brief Variable's max value. Only used for ConVars.
     * @accessors maxValue(), setMaxValue()
     */
    Q_PROPERTY(float maxValue READ maxValue WRITE setMaxValue)
public:
    /**
     * @brief Defines the different types of output that can be displayed in a ConsoleWidget.
     */
    enum OutputType
    {
        OutputGeneral = 0,  /**< Standard message */
        OutputWarning,      /**< Warning */
        OutputCustom1,      /**< Custom type 1 - usually inactive text */
        OutputCustom2,      /**< Custom type 2 - usually highlighted or otherwise important text */
        OutputCustom3,      /**< Custom type 3 */
        OutputCustom4,      /**< Custom type 4 */
        OutputCustom5,      /**< Custom type 5 */
        OutputCustom6       /**< Custom type 6 */
    };
    
    /**
     * @brief Typedef for a callback which accepts output from commands.
     * @param type Type of output to display.
     * @param msg Output message.
     */
    typedef void (CommandManager::*SendOutput) (CommandSenderInfo::OutputType type, const QString &msg);
    
    explicit CommandSenderInfo(QString name, CommandManager *manager, SendOutput outputFunc,
                               bool hasMin = false, float min = 0.0f, bool hasMax = false, float max = 0.0f,
                               QObject *parent = 0);
    
    // Copy constructor
    CommandSenderInfo(const CommandSenderInfo &other);
    
    /**
     * @brief Sends the output through the output callback stored within this object.
     * @param type Type of output to display.
     * @param output Output message.
     */
    void writeOutput(CommandSenderInfo::OutputType type, const QString &output) const;
    
    /**
     * @brief Convenience function - sends a standard message as output.
     * @param message Output message.
     */
    void writeMessage(const QString &message) const;
    
    /**
     * @brief Convenience function - sends a warning message as output.
     * @param message Warning message.
     */
    void writeWarning(const QString &message) const;
    
    /**
     * @brief Returns the pointer to the output callback.
     * @return Callback pointer.
     */
    CommandSenderInfo::SendOutput outputCallback() const;
    
    /**
     * @brief Sets the callback this object will send output through.
     * @param output Pointer to set.
     */
    void setOutputCallback(CommandSenderInfo::SendOutput output);
    
    /**
     * @brief Gets the name of the command this object was assigned to.
     * @return Name of object.
     */
    QString commandName() const;
    
    /**
     * @brief Sets the name of the command this object is assigned to.
     * @param name Name to set.
     */
    void setCommandName(QString &name);
    
    /**
     * @brief Gets the manager this object is assigned to.
     * @return Pointer to manager.
     */
    const CommandManager* commandManager() const;
    
    /**
     * @brief Sets the manager this object is assigned to.
     * @param manager Manager to set.
     */
    void setCommandManager(CommandManager* manager);
    
    bool hasMin() const;
    void setHasMin(bool b);
    
    bool hasMax() const;
    void setHasMax(bool b);
    
    float minValue() const;
    void setMinValue(float val);
    
    float maxValue() const;
    void setMaxValue(float val);
    
signals:
    
public slots:
    
private:
    friend class CommandSenderInfo;
    CommandManager* m_pManager;         /**< Manager we are assigned to.  */
    SendOutput      m_pOutput;          /**< Pointer to output callback. */
    QString         m_NameOfCommand;    /**< Name of command we have been assigned to. */
    bool            m_bHasMin;
    float           m_flMinValue;
    bool            m_bHasMax;
    float           m_flMaxValue;
};

#endif // COMMANDSENDERINFO_H

/**@}*/
