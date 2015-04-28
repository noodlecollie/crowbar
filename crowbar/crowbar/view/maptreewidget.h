#ifndef MAPTREEWIDGET_H
#define MAPTREEWIDGET_H

#include "view_global.h"
#include <QTreeWidget>
#include "model_global.h"

MODEL_BEGIN_NAMESPACE
class MapComponent;
MODEL_END_NAMESPACE

VIEW_BEGIN_NAMESPACE

class VIEWSHARED_EXPORT MapTreeWidget : public QTreeWidget
{
public:
    explicit MapTreeWidget(QWidget* parent = 0);
    ~MapTreeWidget();

    void setData(MODEL_NAMESPACE::MapComponent* root);

    QString worldIcon() const;
    void setWorldIcon(const QString &str);

    QString brushIcon() const;
    void setBrushIcon(const QString &str);

    QString componentIcon() const;
    void setComponentIcon(const QString &str);

private:
    void addItemRecursive(MODEL_NAMESPACE::MapComponent *component, QTreeWidgetItem* componentItem);
    QTreeWidgetItem* createItem(MODEL_NAMESPACE::MapComponent* component, QTreeWidgetItem* parent);
    void clearHeldPointers();
    void clearHeldPointersRecursive(QTreeWidgetItem* item);

    QString m_szUrlWorldIcon;
    QString m_szUrlBrushIcon;
    QString m_szUrlComponentIcon;
};

VIEW_END_NAMESPACE

#endif // MAPTREEWIDGET_H
