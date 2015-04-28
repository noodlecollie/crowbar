#ifndef APPLICATIONDATA_H
#define APPLICATIONDATA_H

#include <QObject>
#include <QHash>
#include "mapdocumentcollection.h"
#include "mapdocument.h"
#include <QList>

class ApplicationData : public QObject
{
    Q_OBJECT
public:
    typedef QHash<MODEL_NAMESPACE::MapDocument*, MapDocumentCollection*> DocumentTable;
    explicit ApplicationData(QObject *parent = 0);
    ~ApplicationData();

    MODEL_NAMESPACE::MapDocument* createDocument();

    const QList<MODEL_NAMESPACE::MapDocument*>& documentList() const;

    MapDocumentCollection* documentData(MODEL_NAMESPACE::MapDocument* doc) const;
    MapDocumentCollection* documentData(int index) const;

signals:
    void documentListChanged();

public slots:

private slots:
    void handleDocumentDestroyed(QObject* obj);

private:
    DocumentTable   m_DocumentTable;
    QList<MODEL_NAMESPACE::MapDocument*> m_DocumentList;
};

ApplicationData* appData();

#endif // APPLICATIONDATA_H
