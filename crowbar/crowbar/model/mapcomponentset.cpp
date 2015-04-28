#include "mapcomponentset.h"
#include "mapdocument.h"
#include "mapcomponent.h"

MODEL_BEGIN_NAMESPACE

MapComponentSet::MapComponentSet(MapDocument *parent) : QObject(parent)
{
    m_bRecomputeBBox = true;
}

MapComponentSet::~MapComponentSet()
{
    clear();
}

MapDocument* MapComponentSet::document() const
{
    Q_ASSERT(qobject_cast<MapDocument*>(parent()));
    return qobject_cast<MapDocument*>(parent());
}

void MapComponentSet::add(MapComponent *component)
{
    if ( !component ) return;
    if ( component->document() != document() || contains(component) ) return;
    
    m_ComponentTable.insert(component);
    connect(component, SIGNAL(renderDataChanged(ChangeId_t)), this, SLOT(flagRecomputeBounds()));
    connect(component, SIGNAL(renderDataChanged(ChangeId_t)), this, SLOT(handleItemChanged()));

    QBox3D bbox = globalBoundingBox();
    if ( bbox.united(component->globalBoundingBox()) != bbox ) flagRecomputeBounds();

    emit itemAdded(component);
}

void MapComponentSet::add(const QList<MapComponent *> &list)
{
    foreach ( MapComponent* c, list )
    {
        add(c);
    }
}

bool MapComponentSet::contains(MapComponent *component) const
{
    return m_ComponentTable.contains(component);
}

void MapComponentSet::remove(MapComponent *component)
{
    if ( m_ComponentTable.remove(component) )
    {
        disconnect(component, SIGNAL(renderDataChanged(ChangeId_t)), this, SLOT(flagRecomputeBounds()));
        disconnect(component, SIGNAL(renderDataChanged(ChangeId_t)), this, SLOT(handleItemChanged()));

        // We could do something more clever here, but...
        flagRecomputeBounds();

        emit itemRemoved(component);
    }
}

void MapComponentSet::remove(const QList<MapComponent *> &list)
{
    foreach ( MapComponent* c, list )
    {
        remove(c);
    }
}

MapComponentSet::Set::iterator MapComponentSet::begin()
{
    return m_ComponentTable.begin();
}

MapComponentSet::Set::iterator MapComponentSet::end()
{
    return m_ComponentTable.end();
}

MapComponentSet::Set::const_iterator MapComponentSet::cbegin() const
{
    return m_ComponentTable.cbegin();
}

MapComponentSet::Set::const_iterator MapComponentSet::cend() const
{
    return m_ComponentTable.cend();
}

QList<MapComponent*> MapComponentSet::items() const
{
    return m_ComponentTable.toList();
}

void MapComponentSet::clear()
{
    // We do this to ensure that itemRemoved() is fired in order for all items.
    QList<MapComponent*> list = items();
    
    foreach ( MapComponent* c, list )
    {
        remove(c);
    }
}

const MapComponentSet::Set& MapComponentSet::constData() const
{
    return m_ComponentTable;
}

int MapComponentSet::count() const
{
    return m_ComponentTable.count();
}

QBox3D MapComponentSet::globalBoundingBox() const
{
    if ( m_bRecomputeBBox )
    {
        m_Bounds = computeBoundingBox();
        m_bRecomputeBBox = false;
    }

    return m_Bounds;
}

QBox3D MapComponentSet::computeBoundingBox() const
{
    Set::const_iterator it = cbegin();
    if ( it == cend() ) return QBox3D();
    QBox3D box = (*it)->globalBoundingBox();
    ++it;

    for ( ; it != cend(); ++it )
    {
        box.unite((*it)->globalBoundingBox());
    }

    return box;
}

QVector3D MapComponentSet::globalCentroid() const
{
    return globalBoundingBox().center();
}

void MapComponentSet::handleItemChanged()
{
    emit itemChanged(qobject_cast<MapComponent*>(QObject::sender()));
}

MODEL_END_NAMESPACE
