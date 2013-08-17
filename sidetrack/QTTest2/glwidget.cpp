#include <QApplication>
#include <QKeyEvent>
#include <QString>

#include "glwidget.h"
#include "cube.h"

GLWidget::GLWidget( const QGLFormat& format, QWidget* parent )
    : QGLWidget( format, parent ),
      m_vertexBuffer( QGLBuffer::VertexBuffer ),
      m_indexBuffer( QGLBuffer::IndexBuffer )
{
}

void GLWidget::initializeGL()
{
    QGLFormat glFormat = QGLWidget::format();
    if ( !glFormat.sampleBuffers() )
        qWarning() << "Could not enable sample buffers";

    // Set the clear color to black
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Prepare a complete shader program...
    QString vert = QCoreApplication::applicationDirPath() + "/simple.vert";
    QString frag = QCoreApplication::applicationDirPath() + "/simple.frag";
    if ( !prepareShaderProgram( vert, frag ) )
        return;
    
    // Bind the shader program so that we can associate variables from
    // our application to the shaders
    if ( !m_shader.bind() )
    {
        qWarning() << "Could not bind shader program to context";
        return;
    }

    // We need us some vertex data. Start simple with a triangle ;-)
    GLfloat* points = tri;
    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
    if ( !m_vertexBuffer.bind() )
    {
        qWarning() << "Could not bind vertex buffer to the context";
        return;
    }
    m_vertexBuffer.allocate( points, SIZEOF_TRI );
    
//    GLfloat* p = points;
//    for ( int i = 0; i < 2*3*3; i++ )
//    {
//        QString str = QString("%0").arg(*p);
//        QByteArray ba = str.toLocal8Bit();
//        qDebug(ba.constData());
//        p++;
//    }
    
    // Indices
    GLuint* indices = tri_ind;
    m_indexBuffer.create();
    m_indexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
    if ( !m_indexBuffer.bind() )
    {
        qWarning() << "Could not bind vertex buffer to the context";
        return;
    }
    m_indexBuffer.allocate( indices, SIZEOF_TRI_IND );
    

    // Enable the "vertex" attribute to bind it to our currently bound
    // vertex buffer.
    m_shader.setAttributeBuffer( "vertex", GL_FLOAT, 0, 3, 6 * sizeof(GLfloat) );
    m_shader.enableAttributeArray( "vertex" );
    m_shader.setAttributeBuffer( "color", GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat) );
    m_shader.enableAttributeArray( "color" );
}

bool GLWidget::prepareShaderProgram( const QString& vertexShaderPath,
                                     const QString& fragmentShaderPath )
{
    // First we load and compile the vertex shader...
    bool result = m_shader.addShaderFromSourceFile( QGLShader::Vertex, vertexShaderPath );
    if ( !result )
        qWarning() << m_shader.log();

    // ...now the fragment shader...
    result = m_shader.addShaderFromSourceFile( QGLShader::Fragment, fragmentShaderPath );
    if ( !result )
        qWarning() << m_shader.log();

    // ...and finally we link them to resolve any references.
    result = m_shader.link();
    if ( !result )
        qWarning() << "Could not link shader program:" << m_shader.log();

    return result;
}

void GLWidget::resizeGL( int w, int h )
{
    // Set the viewport to window dimensions
    glViewport( 0, 0, w, qMax( h, 1 ) );
    
    glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    glOrtho(-10.0, 10.0, -10.0, 10.0, -5.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::paintGL()
{
    // Clear the buffer with the current clearing color
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Draw stuff
    glLoadIdentity();
    //glTranslatef(10.0, 10.0, 0.0);
    glDrawArrays( GL_TRIANGLES, 0, NUM_TRI_VERTS );
}

void GLWidget::keyPressEvent( QKeyEvent* e )
{
    switch ( e->key() )
    {
        case Qt::Key_Escape:
            QCoreApplication::instance()->quit();
            break;

        default:
            QGLWidget::keyPressEvent( e );
    }
}
