#include "mapsubcomponent.h"
#include "mapdocument.h"

MODEL_BEGIN_NAMESPACE

MapSubComponent::MapSubComponent(MapComponent *parent) :
    QObject(NULL)
{
    setParentComponent(parent);
    m_bSelected = false;
}

MapSubComponent::~MapSubComponent()
{
}

void MapSubComponent::setParentComponent(MapComponent *parent)
{
    MapComponent* old = parentComponent();
    setParent(parent);
    manageParentConnections(old, parent);
}

void MapSubComponent::sendRenderDataChange()
{
    emit renderDataChanged(qrand());
}

MapDocument* MapSubComponent::document() const
{
    return parentComponent() ? parentComponent()->document() : NULL;
}

void MapSubComponent::manageParentConnections(MapComponent *pOld, MapComponent *pNew)
{
    if ( pOld )
    {
        disconnect(this, SIGNAL(renderDataChanged(ChangeId_t)), pOld, SLOT(receiveComponentChange(ChangeId_t)));
    }

    if ( pNew )
    {
        connect(this, SIGNAL(renderDataChanged(ChangeId_t)), pNew, SLOT(receiveComponentChange(ChangeId_t)));
    }
}

bool MapSubComponent::selected() const
{
    return m_bSelected;
}

void MapSubComponent::setSelected(bool sel)
{
    if ( sel == m_bSelected ) return;

    m_bSelected = sel;
    emit selectedStatusChanged(m_bSelected);
    sendRenderDataChange();
}

MODEL_END_NAMESPACE
