#include <QString>
#include <QtTest>
#include "mapdocument.h"
#include "mapbrush.h"
#include "mapdocumentview.h"

using namespace MODEL_NAMESPACE;
using namespace VIEW_NAMESPACE;

class Document_view_test : public QObject
{
    Q_OBJECT

public:
    Document_view_test();

private Q_SLOTS:
    void renderDataForDocument();
};

Document_view_test::Document_view_test()
{
}

void Document_view_test::renderDataForDocument()
{
    MapDocument document;
    MapBrush* brush = new MapBrush(document.world());

    MapDocumentView view(&document);
    QVERIFY2(view.contains(brush), "View does not contain data for added brush.");

    QVERIFY2(view.sceneNode(brush, IRender::Triangles), "Solid render node was not generated.");
    QVERIFY2(view.sceneNode(brush, IRender::Edges), "Wireframe render node was not generated.");
}

QTEST_APPLESS_MAIN(Document_view_test)

#include "tst_document_view_test.moc"
