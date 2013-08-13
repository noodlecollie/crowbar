#include "debugmodel2d.h"

using namespace DebugModel;

DebugModel2D::DebugModel2D()
{
}

void DebugModel2D::draw()
{
    GLfloat v2 [] = {
        -12.0, 5.0,
        -14.0, 6.0,
        -10.0, 6.0,
        -10.0, 8.0,
        -6.0, 6.0,
        -6.0, 8.0,
        -4.0, 5.0,
        -2.0, 6.0,
        -4.0, 2.0,
        -2.0, 1.0,
        -12.0, -6.0,
        -9.0, -6.0,
        -14.0, -8.0,
        -9.0, -8.0,
        -2.0, -6.0,
        -2.0, -8.0
    };

    GLfloat vD [] = {
        2.0, 8.0,
        4.0, 8.0,
        2.0, 6.0,
        4.0, 6.0,
        2.0, -6.0,
        4.0, -6.0,
        2.0, -8.0,
        4.0, -8.0,
        10.0, 6.0,
        10.0, 8.0,
        12.0, 4.0,
        14.0, 4.0,
        12.0, -4.0,
        14.0, -4.0,
        10.0, -6.0,
        10.0, -8.0
    };

    GLubyte i2 [] = {
        0,1,2,3,4,5,6,7,8,9,10,11,
        12,10,13,11,15,14
    };

    GLubyte iD [] = {
        0,1,2,3,4,5,6,7,
        3,1,8,9,10,11,12,13,14,15,5,7
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);

    glPushMatrix();
    glLoadIdentity();
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_INDEX_ARRAY );

    glIndexPointer( GL_UNSIGNED_BYTE, 0, i2 );
    glVertexPointer( 2, GL_FLOAT, 0, v2 );
    glDrawElements(GL_TRIANGLE_STRIP, 12, GL_UNSIGNED_BYTE, i2);
    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, i2 + 12);

    glIndexPointer( GL_UNSIGNED_BYTE, 0, iD );
    glVertexPointer( 2, GL_FLOAT, 0, vD );
    glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_BYTE, iD);
    glDrawElements(GL_TRIANGLE_STRIP, 12, GL_UNSIGNED_BYTE, iD + 8);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glPopMatrix();
}
