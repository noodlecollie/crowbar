#include "mapcomponent.h"

MODEL_BEGIN_NAMESPACE

MapComponent::MapComponent(QObject *parent) :
    QObject(parent), m_iHandle(INVALID_HANDLE), m_bSelected(false)
{
}

void MapComponent::setHandle(MapHandle_t handle)
{
    m_iHandle = handle;
    emit handleChanged(m_iHandle);
}

MapHandle_t MapComponent::handle() const
{
    return m_iHandle;
}

bool MapComponent::selected() const
{
    return m_bSelected;
}

void MapComponent::setSelected(bool isSelected)
{
    m_bSelected = isSelected;
    emit selectedStatusChanged(m_bSelected);
}

MODEL_END_NAMESPACE
