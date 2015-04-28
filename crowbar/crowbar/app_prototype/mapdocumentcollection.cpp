#include "mapdocumentcollection.h"
#include "toolselect.h"

using namespace MODEL_NAMESPACE;
using namespace VIEW_NAMESPACE;
using namespace TOOLS_NAMESPACE;

MapDocumentCollection::MapDocumentCollection(MapDocument *document, QObject *parent) : QObject(parent),
    m_pDocument(document)
{
    Q_ASSERT(!m_pDocument.isNull());
    
    m_pView = new MapDocumentView(m_pDocument.data(), this);

    ToolSelect* t = new ToolSelect(this);
    t->setDocument(m_pDocument.data());
    t->setDragBoxEnabled(true);
    t->setMouseOverCrosshairEnabled(true);
    t->setSnapping(true);
    m_ToolList.append(t);
}

MapDocumentCollection::~MapDocumentCollection()
{
}

MapDocument* MapDocumentCollection::document() const
{
    return m_pDocument.data();
}

MapDocumentView* MapDocumentCollection::documentView() const
{
    return m_pView;
}

const QList<BaseTool*>& MapDocumentCollection::toolList() const
{
    return m_ToolList;
}
