/*! \file viewport2d.h
 * \brief Defines the basic 2D OpenGL viewport class.
 *
 * The 2D viewport is used for displaying orthographic content as opposed to perspective-rendered 3D scenes.
 * This is mostly used for top or side views of objects in the same way as Hammer's 2D viewports. <br>
 *
 * Each 2D viewport has a zoom level. A zoom level of 1.0 means that each pixel in the viewport corresponds to an integer 2D co-ordinate
 * in the scene (ie. a line from (0, 0) to (100, 0) would be 100 pixels long).
 * Higher zoom levels mean that this separation is increased (objects appear larger) and lower zoom levels mean it is decreased (objects appear smaller).
 */

#ifndef VIEWPORT2D_H
#define VIEWPORT2D_H

#include "openglwidget.h"
class QVector2D;

/**
 * @brief Convenience struct to hold four clipping plane values.
 */
struct OrthoProjection
{
    GLdouble left;      /**< Left clipping plane. */
    GLdouble right;     /**< Right clipping plane. */
    GLdouble top;       /**< Top clipping plane. */
    GLdouble bottom;    /**< Bottom clipping plane. */
};

/**
 * @brief Viewport designed to render 2D orthographic content.
 */
class Viewport2D : public OpenGLWidget
{
public:

    /**
     * @brief Returns the left, right, top and bottom clipping planes for a 2D viewport with given dimensions, zoom level and translation.
     *
     * The translation is specified by a QVector2D containing the co-ordinates of the centre of the viewport in relation to the 2D scene. For example,
     * a translation of the viewport 5 units to the left and 2 units up would be represented by the co-ordinate (-5, 2).
     * @param width Width of the viewport.
     * @param height Height of the viewport.
     * @param zoomLevel Zoom level of the viewport.
     * @param translation Translation of the viewport relative to the scene's origin.
     * @return OrthoProjection specifying the clipping planes for this viewport's projection.
     */
    static OrthoProjection ProjectionForViewport(int width, int height, double zoomLevel, QVector2D translation);

    /**
     * @brief Constructor
     * @param parent Parent widget.
     */
    explicit Viewport2D(QWidget *parent = 0);

    /**
     * @brief Called when the widget is resized.
     * @param width New width.
     * @param height New height.
     */
    virtual void resizeGL(int width, int height);

    /**
     * @brief Called when the widget should redraw.
     * To force a redraw, use updateGL();
     */
    virtual void paintGL();

    /**
     * @brief Gets the current zoom level.
     *
     * A zoom level of 1.0 means that each pixel in the viewport corresponds to an integer 2D co-ordinate
     * in the scene (ie. a line from (0, 0) to (100, 0) would be 100 pixels long).
     * Higher zoom levels mean that this separation is increased (objects appear larger)
     * and lower zoom levels mean it is decreased (objects appear smaller).
     * @return Current zoom level.
     */
    inline double getZoomLevel() { return m_dbZoomLevel; }

    /**
     * @brief Returns the viewport's current translation from the origin of the 2D scene.
     * @return 2D vector representing the translation.
     */
    inline QVector2D getTranslation() { return m_vecTranslation; }

public slots:
    /**
     * @brief Sets the zoom level for this viewport.
     * @param level Zoom level to set.
     */
    inline void setZoomLevel(double level) { m_dbZoomLevel = level; }

    /**
     * @brief Sets the viewport's stranslation from the origin of the 2D scene.
     * @param translation Translation to set.
     */
    inline void setTranslation(QVector2D translation) { m_vecTranslation = translation; }

private:
    /**
     * @brief Sets the viewport's projection, automatically using the current zoom level and translation of this viewport.
     *
     * This is a convenience function that is called when the viewport is resized.
     * @param width Desired viewport width to use.
     * @param height Desired viewport height to use.
     */
    void setProjection(int width, int height);

    /**
     * @brief Sets the viewport's projection using the dimensions, zoom and translation values from within the viewport.
     *
     * This is a convenience function that is equivalent to setProjection(this->width(), this->height()).
     */
    void setProjection();

    double m_dbZoomLevel;       /**< Zoom level for the viewport. */
    QVector2D m_vecTranslation; /**< Viewport's current translation from the 2D scene's origin.*/
};

#endif // VIEWPORT2D_H
