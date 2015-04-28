#ifndef MAPDOCUMENTCOLLECTION_H
#define MAPDOCUMENTCOLLECTION_H

#include <QObject>
#include "mapdocument.h"
#include "mapdocumentview.h"
#include <QList>
#include "tools_global.h"

TOOLS_BEGIN_NAMESPACE
class BaseTool;
TOOLS_END_NAMESPACE

class MapDocumentCollection : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MODEL_NAMESPACE::MapDocument* document READ document)
    Q_PROPERTY(VIEW_NAMESPACE::MapDocumentView* documentView READ documentView)
public:
    explicit MapDocumentCollection(MODEL_NAMESPACE::MapDocument* document, QObject *parent = 0);
    ~MapDocumentCollection();
    
    MODEL_NAMESPACE::MapDocument*   document() const;
    
    // Things below are owned by us.
    
    VIEW_NAMESPACE::MapDocumentView*    documentView() const;
    const QList<TOOLS_NAMESPACE::BaseTool*>& toolList() const;
    
signals:
    
public slots:
    
private:
    QPointer<MODEL_NAMESPACE::MapDocument>  m_pDocument;
    
    VIEW_NAMESPACE::MapDocumentView*    m_pView;
    QList<TOOLS_NAMESPACE::BaseTool*>   m_ToolList;
};

#endif // MAPDOCUMENTCOLLECTION_H
