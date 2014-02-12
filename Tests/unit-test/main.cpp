#include <QGuiApplication>
#include "octree.h"
#include "worldculltree.h"
#include <Qt3D/QBox3D>
#include <QTime>

using namespace NOctree;

// Gonna keep tests in case they're useful later. Uncomment defines here to activate the tests.
//#define OCTREE_TEST1
//#define OCTREE_TEST2
#define OCTREE_TEST3

#ifdef OCTREE_TEST1
void Octree_Test1()
{
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
    
    // Create one that completely encases some of the frustum
    QBox3D bbox4(QVector3D(-1.5f, -1.2f, -0.1f), QVector3D(1.5f, 1.8f, 0.5f));
    
    // Test it.
    encloseVal = testme.bboxTest(bbox4, fr);
    qDebug("Result of bbox test: %d (expected: %d)", encloseVal, WorldCullTree<int>::FEPartial);
}
#endif  // OCTREE_TEST1

#ifdef OCTREE_TEST2
void Octree_Test2()
{
    QTime timer;
    timer.start();
    WorldCullTree<int> testme(256, 1024.0f);
    
    // There are 128 octree nodes between the origin and the outer edge of the tree.
    // Their dimension should be 8.
    qDebug("Octree dimension: %f (expected: 8)", testme.leafNodeDimension());
    
    // Node at (100,100,200) should have a min of (96,96,200) and a max of (104,104,208).
    QBox3D bbox = testme.nodeBbox(QVector3D(100.0f, 100.0f, 200.0f));
    qDebug("(100,100,200): min: %f %f %f, max %f %f %f", bbox.minimum().x(), bbox.minimum().y(), bbox.minimum().z(),
           bbox.maximum().x(), bbox.maximum().y(), bbox.maximum().z());
    qDebug("Octree_Test2: duration: %dms", timer.elapsed());
}
#endif  // OCTREE_TEST2

#ifdef OCTREE_TEST3
void Octree_Test3()
{
    WorldCullTree<int>* test = new WorldCullTree<int>(256, 1024.0f);
    QTime timer;
    test->add(5, QBox3D(QVector3D(100.0f, 100.0f, 100.0f), QVector3D(150.0f, 150.0f, 150.0f)));
    test->add(10, QBox3D(QVector3D(-15.0f, -30.0f, -15.0f), QVector3D(150.0f, 70.0f, 23.0f)));
    test->add(9001, QBox3D(QVector3D(-1024.0f, -1024.0f, -1024.0f), QVector3D(1024.0f, 1024.0f, 1024.0f))); // <- THIS IS VERY BAD INDEED
    
    timer.start();
    delete test;
    test = NULL;
    
    qDebug("Time to destruct WorldCullTree: %dms", timer.elapsed());
}
#endif  // OCTREE_TEST3

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    // Tests!
    QTime timer;
    qDebug("Begin tests!");
    timer.start();
    
#ifdef OCTREE_TEST1
    Octree_Test1();
#endif  // OCTREE_TEST1
    
#ifdef OCTREE_TEST2
    Octree_Test2();
#endif  // OCTREE_TEST2
    
#ifdef OCTREE_TEST3
    Octree_Test3();
#endif  // OCTREE_TEST3
    
    qDebug("Time elapsed for tests: %dms", timer.elapsed());
    
    //return app.exec();
    return 0;
}
