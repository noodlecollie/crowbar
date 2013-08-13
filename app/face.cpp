#include "face.h"

bool Face3D::addEdge(const GEOMHANDLE edge, const bool checkForExistence)
{
    int index = -1;

    // Check whether the edge ID already exists.
    if ( checkForExistence )
    {
        index = m_hEdges->indexOf(edge);
    }

    // If we checked and no edge existed, or we didn't check at all,
    // we should append to the end of the list.
    if ( index == -1 )
    {
        m_hEdges->append(edge);
        return true;
    }

    // If the edge does already exist, don't add.
    return false;
}
