#include "mapworld.h"
#include "mapdocument.h"

MODEL_BEGIN_NAMESPACE

MapWorld::MapWorld(MapDocument *document) : MapComponent(NULL)
{
    updateVirtualMetaInfo();

    // We set the document and parent manually.
    // This means our parentComponent is NULL, but our QObject parent is the document.
    setDocument(document);
    setParent(document);
}

MapWorld::~MapWorld()
{
    baseCleanup();
}

MODEL_END_NAMESPACE

