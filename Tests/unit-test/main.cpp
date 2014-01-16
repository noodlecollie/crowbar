#include <QGuiApplication>
#include "octree.h"

using namespace NOctree;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
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
    
    //return app.exec();
    return 0;
}
