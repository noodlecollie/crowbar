#ifndef COMMANDSUGGESTIONLIST_H
#define COMMANDSUGGESTIONLIST_H

#include "iconsole_global.h"
#include <QListWidget>

class QString;

class ICONSOLESHARED_EXPORT CommandSuggestionList : public QListWidget
{
    Q_OBJECT
    Q_PROPERTY(float heightScale READ heightScale WRITE setHeightScale RESET resetHeightScale NOTIFY heightScaleChanged)
public:
    explicit CommandSuggestionList(QWidget *parent = 0);
    virtual ~CommandSuggestionList() {}
    
    QString getCurrentSelection();
    bool hasSelection() const;

    float heightScale() const;
    void setHeightScale(float scale);
    void resetHeightScale();
    
signals:
    void heightScaleChanged();
    //void itemDoubleClicked(QListWidgetItem *);
    
public slots:
    void moveUp();
    void moveDown();
    void selectFirst();
    void autoHeight();
    void autoWidth();
    
private:
    float m_flHeightScale;
};

#endif // COMMANDSUGGESTIONLIST_H
