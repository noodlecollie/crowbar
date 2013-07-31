#include "viewport2d.h"
#include <QVector2D>

OrthoProjection Viewport2D::ProjectionForViewport(int width, int height, double zoomLevel, QVector2D translation)
{
    OrthoProjection p;

    // The plane is specified by halving either the width or the height, negating if necessary and
    // dividing by the zoom level. The translation constant is then added.

    double temp = ((double)width/(2.0 * zoomLevel));
    p.right = temp + translation.x();
    p.left = -temp + translation.x();

    temp = ((double)height/(2.0 * zoomLevel));
    p.top = temp + translation.y();
    p.bottom = -temp + translation.y();

    return p;
}

Viewport2D::Viewport2D(QWidget *parent) : OpenGLWidget(parent)
{
    m_vecTranslation.setX(0.0);
    m_vecTranslation.setY(0.0);
    m_dbZoomLevel = 1.0;
}

void Viewport2D::resizeGL(int width, int height)
{
    // Do the base resize first.
    OpenGLWidget::resizeGL(width, height);

    glMatrixMode(GL_PROJECTION);            // Projection mode.
    glLoadIdentity();                       // Clear out previous matrix.
    setProjection(width, height);           // Set new projection.
    glMatrixMode(GL_MODELVIEW);             // Back to model view mode.
}

void Viewport2D::setProjection(int width, int height)
{
    OrthoProjection p = ProjectionForViewport(width, height, getZoomLevel(), getTranslation());
    glOrtho(p.left, p.right, p.bottom, p.top, -1, 1);       // Set new projection.
}

// Used when we need a new projection (eg. after a translation) but want to use our current
// width and height values.
void Viewport2D::setProjection()
{
    setProjection(this->width(), this->height());
}

void Viewport2D::paintGL()
{
    // TEST
//    // We're drawin' a triangle!
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glColor3f(1.0, 0.0, 0.0);

//    glLoadIdentity();
//    GLfloat verts[] = {-2.0, 0.0, 0.0, 4.0, 2.0, 0.0};
//    GLushort indices[] = {0, 1, 2};
//    glEnableClientState( GL_VERTEX_ARRAY );
//    glEnableClientState( GL_INDEX_ARRAY );

//    glIndexPointer( GL_UNSIGNED_BYTE, 0, indices );
//    glVertexPointer( 2, GL_FLOAT, 0, verts );
//    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, indices);
//    glDisableClientState(GL_VERTEX_ARRAY);
//    glDisableClientState(GL_NORMAL_ARRAY);
    // TEST
}
