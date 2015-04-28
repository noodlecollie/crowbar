#ifndef RENDERENTRYFACTORY_H
#define RENDERENTRYFACTORY_H

#include "view_global.h"
#include "model_global.h"
#include <QHash>

MODEL_BEGIN_NAMESPACE
class MapComponent;
MODEL_END_NAMESPACE

VIEW_BEGIN_NAMESPACE

class RenderTableEntry;
class MapDocumentView;

namespace RenderTableEntries
{
    typedef RenderTableEntry* (*RenderTableFactory_t)(MODEL_NAMESPACE::MapComponent*);
    typedef QHash<QString, RenderTableFactory_t> FactoryTable;

    // Define a new function for each type of component.
    RenderTableEntry* brushEntry(MODEL_NAMESPACE::MapComponent* component);
    RenderTableEntry* positionalLightEntry(MODEL_NAMESPACE::MapComponent* component);
    RenderTableEntry* toolBasicSquareEntry(MODEL_NAMESPACE::MapComponent* component);
    RenderTableEntry* toolWireframeBoxEntry(MODEL_NAMESPACE::MapComponent* component);
    
    void initialiseTable(FactoryTable &table);
    const FactoryTable& factoryTable();
    RenderTableEntry* entryFor(MODEL_NAMESPACE::MapComponent* component);
}

VIEW_END_NAMESPACE

#endif // RENDERENTRYFACTORY_H

