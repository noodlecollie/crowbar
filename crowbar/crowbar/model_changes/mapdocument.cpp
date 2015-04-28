#include "mapdocument.h"
#include "mapbrush.h"
#include <QtDebug>

QDebug operator<<(QDebug dbg, const MODEL_NAMESPACE::MapDocument &doc)
{
    dbg.nospace() << &doc <<  "\nBrushes:\n";
    
    if ( doc.brushes().count() < 1 ) dbg.nospace() << "None.\n";
    else
    {
        for ( int i = 0; i < doc.brushes().count(); i++ )
        {
            dbg.nospace() << *doc.brushes().at(i);
        }
    }
                     
    return dbg.space();
}

MODEL_BEGIN_NAMESPACE

MapDocument::MapDocument(QObject *parent) : QObject(parent),
    m_BrushList(BRUSHES_TITLE, this)
{
    // Connect signals for when brushes are added or removed.
    connect(&m_BrushList.signalRelay, &ObjectListSignals::itemAddedMapBrush, this, &MapDocument::brushAdded);
    connect(&m_BrushList.signalRelay, &ObjectListSignals::itemRemovedMapBrush, this, &MapDocument::brushRemoved);
}

MapDocument::~MapDocument()
{
    qDebug("Destruction of MapDocument");
    
    disconnect(&m_BrushList.signalRelay, &ObjectListSignals::itemAddedMapBrush, this, &MapDocument::brushAdded);
    disconnect(&m_BrushList.signalRelay, &ObjectListSignals::itemRemovedMapBrush, this, &MapDocument::brushRemoved);

    m_BrushList.clear();
}

void MapDocument::setTitle(const QString &title)
{
    m_szTitle = title;
}

QString MapDocument::title() const
{
    return m_szTitle;
    emit titleChanged(m_szTitle);
}

MapDocument::MapBrushList& MapDocument::brushes()
{
    return m_BrushList;
}

const MapDocument::MapBrushList& MapDocument::brushes() const
{
    return m_BrushList;
}

void MapDocument::handleBrushAdded(MapBrush *brush)
{
    brush->setDocument(this);
    emit brushAdded(brush);
}

void MapDocument::handleBrushRemoved(MapBrush *brush)
{
    brush->setDocument(NULL);
    emit brushRemoved(brush);
}

MODEL_END_NAMESPACE
