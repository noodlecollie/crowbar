#include "renderentryfactory.h"
#include "mapbrush.h"
#include "renderentrymapbrush.h"
#include <QtDebug>
#include "model_util.h"
#include "maplightpositional.h"
#include "renderentrymaplightpositional.h"
#include "renderentrytoolbasicsquare.h"
#include "renderentrytoolwireframebox.h"

using namespace MODEL_NAMESPACE;

VIEW_BEGIN_NAMESPACE

namespace RenderTableEntries
{
    const FactoryTable& factoryTable()
    {
        static bool initialised = false;
        static FactoryTable table;
        
        if ( !initialised )
        {
            initialiseTable(table);
            initialised = true;
        }
        
        return table;
    }
    
    void initialiseTable(FactoryTable &table)
    {
        // MapBrush
        table.insert(CLASSNAME(MapBrush), &brushEntry);
        
        // MapLightPositional
        table.insert(CLASSNAME(MapLightPositional), &positionalLightEntry);

        // ToolBasicSquare
        table.insert(CLASSNAME(ToolBasicSquare), &toolBasicSquareEntry);

        // ToolWireframeBox
        table.insert(CLASSNAME(ToolWireframeBox), &toolWireframeBoxEntry);
    }
    
    RenderTableEntry* brushEntry(MapComponent *component)
    {
        Q_ASSERT(qobject_cast<MapBrush*>(component));
        return new RenderEntryMapBrush(qobject_cast<MapBrush*>(component));
    }
    
    RenderTableEntry* positionalLightEntry(MapComponent *component)
    {
        Q_ASSERT(qobject_cast<MapLightPositional*>(component));
        return new RenderEntryMapLightPositional(qobject_cast<MapLightPositional*>(component));
    }

    RenderTableEntry* toolBasicSquareEntry(MapComponent *component)
    {
        Q_ASSERT(qobject_cast<ToolBasicSquare*>(component));
        return new RenderEntryToolBasicSquare(qobject_cast<ToolBasicSquare*>(component));
    }

    RenderTableEntry* toolWireframeBoxEntry(MapComponent *component)
    {
        Q_ASSERT(qobject_cast<ToolWireframeBox*>(component));
        return new RenderEntryToolWireframeBox(qobject_cast<ToolWireframeBox*>(component));
    }
    
    RenderTableEntry* entryFor(MapComponent *component)
    {
        RenderTableFactory_t func = factoryTable().value(component->metaObject()->className(), NULL);
        if ( !func )
        {
            qWarning() << "RenderTableEntries: No factory method present for component"
                       << component->metaObject()->className();

            return new RenderTableEntry(component);
        }

        RenderTableEntry* e = (*func)(component);
        return e ? e : new RenderTableEntry(component);
    }
}

VIEW_END_NAMESPACE
