#include "maptreewidget.h"
#include "mapcomponent.h"
#include "mapworld.h"
#include "mapbrush.h"
#include <QIcon>

using namespace MODEL_NAMESPACE;

VIEW_BEGIN_NAMESPACE

MapTreeWidget::MapTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setColumnCount(1);
    setHeaderHidden(true);
}

MapTreeWidget::~MapTreeWidget()
{
    clearHeldPointers();
}

void MapTreeWidget::setData(MapComponent *root)
{
    clearHeldPointers();
    clear();
    if ( !root ) return;

    // Create a top-level item for the root.
    QTreeWidgetItem* item = createItem(root, NULL);

    // Populate recursively.
    addItemRecursive(root, item);

    addTopLevelItem(item);
}

void MapTreeWidget::addItemRecursive(MapComponent *component, QTreeWidgetItem* componentItem)
{
    // Get a list of the component's children.
    QList<MapComponent*> children = component->childComponents();

    // For each child, generate an item for it, add it to the component's item
    // and call recursively.
    foreach ( MapComponent* c, children )
    {
        QTreeWidgetItem* item = createItem(c, componentItem);
        addItemRecursive(c, item);
    }
}

QTreeWidgetItem* MapTreeWidget::createItem(MapComponent *component, QTreeWidgetItem *parent)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(parent);
    QString name = component->objectName();
    QString display = QString("0x%0").arg(QString::number((qint64)component, 16));

    if ( !name.isEmpty() )
    {
        display += QString(" (%0)").arg(name);
    }
    else
    {
        QVector3D position = component->hierarchyTransform()->translation();
        display += QString(" [%0 %1 %2]").arg(position.x()).arg(position.y()).arg(position.z());
    }

    item->setData(0, Qt::DisplayRole, display);

    // Terrible, awful way of dealing with this but it works.
    QPointer<MapComponent>* pointer = new QPointer<MapComponent>(component);
    item->setData(0, Qt::UserRole, reinterpret_cast<qint64>(pointer));

    if ( qobject_cast<MapWorld*>(component) && !m_szUrlWorldIcon.isEmpty() )
    {
        item->setData(0, Qt::DecorationRole, QImage(m_szUrlWorldIcon));
    }
    else if ( qobject_cast<MapBrush*>(component) && !m_szUrlBrushIcon.isEmpty() )
    {
        item->setData(0, Qt::DecorationRole, QImage(m_szUrlBrushIcon));
    }
    else if ( !m_szUrlComponentIcon.isEmpty() )
    {
        item->setData(0, Qt::DecorationRole, QImage(m_szUrlComponentIcon));
    }

    return item;
}

QString MapTreeWidget::worldIcon() const
{
    return m_szUrlWorldIcon;
}

void MapTreeWidget::setWorldIcon(const QString &str)
{
    if ( str == m_szUrlWorldIcon ) return;

    m_szUrlWorldIcon = str;
}

QString MapTreeWidget::brushIcon() const
{
    return m_szUrlBrushIcon;
}

void MapTreeWidget::setBrushIcon(const QString &str)
{
    if ( str == m_szUrlBrushIcon ) return;

    m_szUrlBrushIcon = str;
}

QString MapTreeWidget::componentIcon() const
{
    return m_szUrlComponentIcon;
}

void MapTreeWidget::setComponentIcon(const QString &str)
{
    if ( str == m_szUrlComponentIcon ) return;

    m_szUrlComponentIcon = str;
}

void MapTreeWidget::clearHeldPointers()
{
    for ( int i = 0; i < topLevelItemCount(); i++ )
    {
        clearHeldPointersRecursive(topLevelItem(i));
    }
}

void MapTreeWidget::clearHeldPointersRecursive(QTreeWidgetItem *item)
{
    if ( !item ) return;

    QPointer<MapComponent>* pointer = reinterpret_cast<QPointer<MapComponent>*>(item->data(0, Qt::UserRole).value<qint64>());
    if ( pointer ) delete pointer;
    item->setData(0, Qt::UserRole, 0);

    for ( int i = 0; i < item->childCount(); i++ )
    {
        clearHeldPointersRecursive(item->child(i));
    }
}

VIEW_END_NAMESPACE
