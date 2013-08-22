#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

// OLD CODE
#if 0
#include <QtOpenGL>
#include <QGLWidget>
#include <QList>
#include <QPair>

typedef QPair<GLenum, GLenum> GLenumPair;
typedef QList<GLenum> GLEnumList;
typedef QList<GLenumPair> GLenumPairList;

/**
 * @brief Defines a struct to hold OpenGL widget initialisation settings.
 */
struct OpenGLInitialiser
{
    GLenum              m_iShadeModel;  /**< Shade model - smooth or flat. */
    QColor              m_ClearColour;  /**< Colour to clear the back buffer to. */
    GLEnumList          m_iEnables;     /**< Each item in this list will be enabled by OpenGL. */
    GLenum              m_iDepthFunc;   /**< Depth buffer function. */
    GLenumPairList      m_Hints;        /**< Hints to pass to glHint. */
};

/**
 * @brief Defines an OpenGL widget.
 *
 * This class reimplements QGLWidget to provide an OpenGL viewport with which
 * to render scenes.
 */
class OpenGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor.
     * @param parent Parent object.
     */
    explicit OpenGLWidget(QWidget *parent = 0);

    /**
      @brief Destructor.
    */
    ~OpenGLWidget();
    
signals:
    
public slots:
    /**
     * @brief Sets the initialiser struct for the viewport.
     * @param init Struct to set.
     */
    inline void setInitialiser(OpenGLInitialiser init) { m_Initialiser = init; m_bInitialiserSet = true; }

protected:
    /**
     * @brief Initialises the GL context.
     */
    void initializeGL();

    /**
     * @brief Called when the widget is resized.
     * @param width New width.
     * @param height New height.
     */
    virtual void resizeGL(int width, int height);

private:
    OpenGLInitialiser m_Initialiser;    /**< Holds the initialisation settings. */
    bool m_bInitialiserSet;             /**< Set to true if the OpenGL intialiser has been set. */
};

#endif // OPENGLWIDGET_H
#endif // 0
