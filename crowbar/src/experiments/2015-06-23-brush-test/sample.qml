import QtQuick 2.0
import crowbar.experiments.qml 1.0

Brush
{
    vertices:
    [
        BrushVertex // 0
        {
            position: Qt.vector3d(0,2,0)
        },

        BrushVertex // 1
        {
            position: Qt.vector3d(4,2,0)
        },

        BrushVertex // 2
        {
            position: Qt.vector3d(4,0,0);
        },

        BrushVertex // 3
        {
            position: Qt.vector3d(0,0,0);
        },

        BrushVertex // 4
        {
            position: Qt.vector3d(0,2,2);
        },

        BrushVertex // 5
        {
            position: Qt.vector3d(4,2,2);
        },

        BrushVertex // 6
        {
            position: Qt.vector3d(4,0,2);
        },

        BrushVertex // 7
        {
            position: Qt.vector3d(0,0,2);
        }
    ]

    faces:
    [
        BrushFace
        {
            vertexIndices: [ 0,1,2,3 ]
        },

        BrushFace
        {
            vertexIndices: [ 4,7,6,5 ]
        },

        BrushFace
        {
            vertexIndices: [ 0,3,7,4 ]
        },

        BrushFace
        {
            vertexIndices: [ 2,1,5,6 ]
        },

        BrushFace
        {
            vertexIndices: [ 3,2,6,7 ]
        },

        BrushFace
        {
            vertexIndices: [ 0,4,5,1 ]
        }
    ]
}
