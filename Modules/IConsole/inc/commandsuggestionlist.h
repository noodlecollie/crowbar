/** \file commandsuggestionlist.h
 * \brief Defines the CommandSuggestionList class which holds suggestions for the currently typed command.
 */

/**
 * \defgroup IConsole IConsole library
 * @{
 */

#ifndef COMMANDSUGGESTIONLIST_H
#define COMMANDSUGGESTIONLIST_H

#include "iconsole_global.h"
#include <QListWidget>

class QString;

/**
 * @brief Holds and displays console command suggestions to the user.
 */
class ICONSOLESHARED_EXPORT CommandSuggestionList : public QListWidget
{
    Q_OBJECT
    /** @property heightScale
     * @brief Scaling factor for height of suggestion box depending on how many items it contains.
     * @accessors heightScale(), setHeightScale(), resetHeightScale(), heightScaleChanged()
     */
    Q_PROPERTY(float heightScale READ heightScale WRITE setHeightScale RESET resetHeightScale NOTIFY heightScaleChanged)
public:
    
    /**
     * @brief Constructor.
     * @param parent QWidget parent, if applicable.
     */
    explicit CommandSuggestionList(QWidget *parent = 0);
    
    /**
     * @brief Destructor.
     */
    virtual ~CommandSuggestionList() {}
    
    /**
     * @brief Returns a string representing the current selection.
     * @return Current selection string, or empty string if there is no selection.
     */
    QString getCurrentSelection();
    
    /**
     * @brief Returns whether the suggestion box has a current selection.
     * @return True if an item is selected, false otherwise.
     */
    bool hasSelection() const;

    /**
     * @brief Gets the height scale for the suggestion box.
     * @return Height scale.
     */
    float heightScale() const;
    
    /**
     * @brief Sets the height scale for the suggestion box.
     * @param scale Scale to set.
     */
    void setHeightScale(float scale);
    
    /**
     * @brief Resets the height scale to the default.
     */
    void resetHeightScale();
    
signals:
    /**
     * @brief Emitted when the height scale property is changed.
     */
    void heightScaleChanged();
    //void itemDoubleClicked(QListWidgetItem *);
    
public slots:
    /**
     * @brief Moves the selection up one item.
     */
    void moveUp();
    
    /**
     * @brief Moves the selection up one item.
     */
    void moveDown();
    
    /**
     * @brief Selects the first item.
     */
    void selectFirst();
    
    /**
     * @brief Attempts to aut-size the suggestion box's height.
     */
    void autoHeight();
    
    /**
     * @brief Attempts to aut-size the suggestion box's width.
     */
    void autoWidth();
    
private:
    float m_flHeightScale;  /**< Height scale for the suggestion box */
};

#endif // COMMANDSUGGESTIONLIST_H

/**@}*/
