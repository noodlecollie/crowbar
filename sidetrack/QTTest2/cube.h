#ifndef CUBE_H
#define CUBE_H

#include <QGLWidget>

// XYZRGB
static GLfloat cube_verts[] = {
    -1.0, -1.0, 1.0,    1.0, 0.0, 0.0,  // 0
    -1.0, 1.0, 1.0,     1.0, 1.0, 0.0,  // 1
    1.0, 1.0, 1,.0,     0.0, 1.0, 1.0,  // 2
    1.0, -1.0, 1.0,     0.0, 0.0, 1.0,  // 3
    
    -1.0, -1.0, -1.0,   0.0, 1.0, 1.0,  // 4
    -1.0, 1.0, -1.0,    0.0, 0.0, 1.0,  // 5
    1.0, 1.0, -1,.0,    1.0, 0.0, 0.0,  // 6
    1.0, -1.0, -1.0,    1.0, 1.0, 0.0   // 7
};

static GLuint cube_indices[] = {
    0,1,2,
    0,2,3,
    0,3,4,
    3,7,4,
    3,2,7,
    2,6,7,
    1,0,5,
    0,4,5,
    2,1,6,
    5,6,1,
    6,5,7,
    4,7,5
};

static GLfloat tri[] = { -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
                        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
                        0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f };

static GLfloat tri_old[] = { -0.5f, -0.5f, 0.0f,
                             0.5f, -0.5f, 0.0f,
                             0.0f,  0.5f, 0.0f, };

static GLuint tri_ind[] = { 0, 1, 2 };

static int SIZEOF_CUBEVERTS = 3 * 4 * 4 * sizeof(GLfloat);
static int SIZEOF_CUBEINDS = 3 * 12 * sizeof(GLuint);
static int NUM_CUBE_VERTS = 36;

static int SIZEOF_TRI = 2 * 3 * 3 * sizeof(GLfloat);
static int SIZEOF_TRI_OLD = 3 * 3 * sizeof(GLfloat);
static int SIZEOF_TRI_IND = 3 * sizeof(GLuint);
static int NUM_TRI_VERTS = 3;

#endif // CUBE_H
