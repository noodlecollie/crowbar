#include <QString>
#include <QtTest>
#include "mapdocument.h"
#include "mapcomponent.h"
#include "mapbrush.h"
#include <QPointer>
#include <QMatrix4x4>
#include <QGraphicsTranslation3D>
using namespace MODEL_NAMESPACE;

class New_component_tests : public QObject
{
    Q_OBJECT

public:
    New_component_tests();

private Q_SLOTS:
    void treeParenting();
    void treeParentDocument();
    void componentDeletedWhenDocumentKilled();
    void brushPutIntoBrushList();
    void componentHierarchy();
};

New_component_tests::New_component_tests()
{
}

void New_component_tests::treeParenting()
{
    MapDocument document;
    MapComponent* component = new MapComponent(document.world());

    QVERIFY2(component->parentComponent() == document.world(), "Component did not parent correctly.");
    delete component;
}

void New_component_tests::treeParentDocument()
{
    MapDocument document;
    MapComponent* component = new MapComponent(document.world());

    QVERIFY2(component->document() == &document, "Component's document was not set correctly.");
    delete component;
}

void New_component_tests::componentDeletedWhenDocumentKilled()
{
    MapDocument* document = new MapDocument();
    QPointer<MapComponent> component = new MapComponent(document->world());
    delete document;
    document = NULL;

    QVERIFY2(component.isNull(), "Component not deleted on destruction of document.");
}

void New_component_tests::brushPutIntoBrushList()
{
    MapDocument document;
    MapBrush* brush = new MapBrush(document.world());
    qDebug() << "Brush pointer:" << brush;

    QVERIFY2(document.brushes().contains(brush), "Brush not added to document brush list.");
}

void New_component_tests::componentHierarchy()
{
    MapDocument document;
    MapBrush* brush = new MapBrush(document.world());
    brush->newVertex(QVector3D(3,0,0));
    
    document.world()->hierarchyTransform()->setTranslation(QVector3D(5,0,0));
    
    QMatrix4x4 mat = brush->hierarchyTransform()->transformMatrix() * document.world()->hierarchyTransform()->transformMatrix();
    QVector4D v4(3,0,0,1);
    v4 = mat * v4;
    QVector3D v3(v4.x(), v4.y(), v4.z());
    QVERIFY2(v3 == QVector3D(8,0,0), "Matrix transforms not computed correctly");
}

QTEST_APPLESS_MAIN(New_component_tests)

#include "tst_new_component_tests.moc"
