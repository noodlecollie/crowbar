#include "applicationdata.h"
#include <QApplication>
#include <QPointer>

using namespace MODEL_NAMESPACE;

static QPointer<ApplicationData> g_pAppData;

ApplicationData* appData()
{
    if ( g_pAppData.isNull() && qApp )
    {
        g_pAppData = QPointer<ApplicationData>(new ApplicationData(qApp));
    }

    return g_pAppData.data();
}

ApplicationData::ApplicationData(QObject *parent) :
    QObject(parent)
{
}

ApplicationData::~ApplicationData()
{
}

const QList<MapDocument*>& ApplicationData::documentList() const
{
    return m_DocumentList;
}

MapDocument* ApplicationData::createDocument()
{
    MapDocument* doc = new MapDocument(this);   // Document deleted when ApplicationData is deleted.
    connect(doc, SIGNAL(destroyed(QObject*)), this, SLOT(handleDocumentDestroyed(QObject*)));

    m_DocumentTable.insert(doc, new MapDocumentCollection(doc, this));
    m_DocumentList.append(doc);
    emit documentListChanged();

    return doc;
}

void ApplicationData::handleDocumentDestroyed(QObject *obj)
{
    MapDocument* doc = qobject_cast<MapDocument*>(obj);
    if ( !doc ) return;

    MapDocumentCollection* val = m_DocumentTable.take(doc);
    delete val;
    val = NULL;

    m_DocumentList.removeOne(doc);
    emit documentListChanged();
}

MapDocumentCollection* ApplicationData::documentData(MapDocument* doc) const
{
    return m_DocumentTable.value(doc, NULL);
}

MapDocumentCollection* ApplicationData::documentData(int index) const
{
    return documentData(documentList().at(index));
}
