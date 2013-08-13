#ifndef DEBUGMODEL2D_H
#define DEBUGMODEL2D_H

#include <QtOpenGL>

namespace DebugModel
{
    class DebugModel2D
    {
    public:
        DebugModel2D();
        void draw();

    private:
        GLfloat* verts2;
        GLfloat* vertsD;
        GLubyte* indices2;
        GLubyte* indicesD;
        int sizev2;
        int sizevD;
        int sizei2;
        int sizeiD;
    };
}
#endif // DEBUGMODEL2D_H
