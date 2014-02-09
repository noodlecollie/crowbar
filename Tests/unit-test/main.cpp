#include <QGuiApplication>
#include "octree.h"
#include "worldculltree.h"
#include <Qt3D/QBox3D>

using namespace NOctree;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    WorldCullTree<int> testme(256, 1024);
    
    Octree<int> o(256);
    
    // In this example, each node is 128 units large in each dimension.
    // This makes the total dimension of the overall octree cube 32768 units.
    // Magnitude = 16384.
    
#define MAGNITUDE 16384
    
    // For a co-ordinate in x of 5.1, this should be in node 128.
    qDebug("Expected: 128. Result: %d", o.mapToNodeIndex(MAGNITUDE, 5.1f));
    
    // For a co-ordinate in x of 32768, this should be in node 255.
    qDebug("Expected: 255. Result: %d", o.mapToNodeIndex(MAGNITUDE, 32768.0f));
    
    // Translating the position (-32667.5, 16201.27, 1301.0) to an octree node index:
    // This -should- result in an octree address of (0, 254, 138).
    int nodeX = o.mapToNodeIndex(MAGNITUDE, -32667.5f);
    int nodeY = o.mapToNodeIndex(MAGNITUDE, 16201.27f);
    int nodeZ = o.mapToNodeIndex(MAGNITUDE, 1301.0f);
    
    qDebug("Expected: (0, 254, 138). Result: (%d %d %d)", nodeX, nodeY, nodeZ);
    
    // Insert something into the octree at (0, 254, 138).
    o.set(0, 254, 138, 9001);
    
    // Return the value at (-32667.5, 16201.27, 1301.0).
    qDebug("Getting value at (-32667.5, 16201.27, 1301.0): %d", o.atPosition(MAGNITUDE, QVector3D(-32667.5f, 16201.27f, 1301.0f)));
    
    // Create a frustum cube.
    // -Z is forward, Z is backward; X is upward, Y is left.
    NGeometry::Frustum fr;
    fr.farPlane = QPlane3D(QVector3D(0.0f, 0.0f, -1.0f), QVector3D(0.0f, 0.0f, -1.0f));
    fr.nearPlane = QPlane3D(QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f));
    fr.leftPlane = QPlane3D(QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
    fr.rightPlane = QPlane3D(QVector3D(0.0f, -1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f));
    fr.topPlane = QPlane3D(QVector3D(1.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f));
    fr.bottomPlane = QPlane3D(QVector3D(-1.0f, 0.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f));
    
    // Create a bounding box.
    QBox3D bbox(QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(0.5f, 0.5f, 0.5f));
    
    // Test it.
    WorldCullTree<int>::FrustumEnclose encloseVal = testme.bboxTest(bbox, fr);
    qDebug("Result of bbox test: %d (expected: %d)", encloseVal, WorldCullTree<int>::FEFull);
    
    // Create another.
    QBox3D bbox2(QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(1.5f, 1.5f, 1.5f));
    
    // Test it.
    encloseVal = testme.bboxTest(bbox2, fr);
    qDebug("Result of bbox test: %d (expected: %d)", encloseVal, WorldCullTree<int>::FEPartial);
    
    // Create another.
    QBox3D bbox3(QVector3D(1.1f, 1.1f, 1.1f), QVector3D(1.5f, 1.5f, 1.5f));
    
    // Test it.
    encloseVal = testme.bboxTest(bbox3, fr);
    qDebug("Result of bbox test: %d (expected: %d)", encloseVal, WorldCullTree<int>::FENone);
    
    //return app.exec();
    return 0;
}
