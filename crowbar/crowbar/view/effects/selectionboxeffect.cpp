#include "selectionboxeffect.h"
#include <QGLPainter>
#include <QOpenGLShaderProgram>

// TODO: Should we make these loadable files?
static const char vertexShader[] =
        "attribute highp vec4 vertex;\n"
        "uniform highp mat4 matrix;\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = matrix * vertex;\n"
        "}\n";

static const char fragmentShader[] =
        "uniform mediump vec4 color;\n"
        "void main(void)\n"
        "{\n"
        "    const mediump vec4 space = vec4(0.0,0.0,0.0,1.0);\n"
        "    bool flagX = (mod(gl_FragCoord.x, 8.0) < 4.0);\n"
        "    bool flagY = (mod(gl_FragCoord.y, 8.0) < 4.0);\n"
        "    bool useCol = (flagX && flagY) || (!flagX && !flagY);\n"
        "    gl_FragColor = (useCol) ? color : space;\n"
        "}\n";

#define CACHED_PROGRAM_STRING "crowbar.color.selectionbox"

VIEW_BEGIN_NAMESPACE

SelectionBoxEffect::SelectionBoxEffect() :
    QGLAbstractEffect(), m_pShaderProgram(NULL), m_iMatrixUniform(-1),
    m_iColorUniform(-1), m_bIsFixedFunction(false)
{
}

SelectionBoxEffect::~SelectionBoxEffect()
{
}

bool SelectionBoxEffect::supportsPicking() const
{
    return false;
}

void SelectionBoxEffect::setActive(QGLPainter *painter, bool flag)
{
#if defined(QGL_FIXED_FUNCTION_ONLY)
    Q_UNUSED(painter);
    if (flag)
        glEnableClientState(GL_VERTEX_ARRAY);
    else
        glDisableClientState(GL_VERTEX_ARRAY);
#else
    Q_UNUSED(painter);
#if !defined(QGL_SHADERS_ONLY)
    if (painter->isFixedFunction()) {
        m_bIsFixedFunction = true;
        if (flag)
            glEnableClientState(GL_VERTEX_ARRAY);
        else
            glDisableClientState(GL_VERTEX_ARRAY);
        return;
    }
#endif  // !defined(QGL_SHADERS_ONLY)

    QOpenGLShaderProgram *program =
        painter->cachedProgram(QLatin1String("crowbar.color.selectionbox"));
    m_pShaderProgram = program;
    if (!program) {
        if (!flag)
            return;
        program = new QOpenGLShaderProgram;
        program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader);
        program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader);
        program->bindAttributeLocation("vertex", QGL::Position);
        if (!program->link()) {
            qWarning("SelectionBoxEffect::setActive(): could not link shader program");
            delete program;
            return;
        }
        painter->setCachedProgram(QLatin1String("crowbar.color.selectionbox"), program);
        m_pShaderProgram = program;
        m_iColorUniform = program->uniformLocation("color");
        m_iMatrixUniform = program->uniformLocation("matrix");
        program->bind();
        program->enableAttributeArray(QGL::Position);
    } else if (flag) {
        m_iColorUniform = program->uniformLocation("color");
        m_iMatrixUniform = program->uniformLocation("matrix");
        program->bind();
        program->enableAttributeArray(QGL::Position);
    } else {
        program->disableAttributeArray(QGL::Position);
        program->release();
    }
#endif  // defined(QGL_FIXED_FUNCTION_ONLY)
}

void SelectionBoxEffect::update(QGLPainter *painter, QGLPainter::Updates updates)
{
#if defined(QGL_FIXED_FUNCTION_ONLY)
    painter->updateFixedFunction
        (updates & (QGLPainter::UpdateColor |
                    QGLPainter::UpdateMatrices));
#else
#if !defined(QGL_SHADERS_ONLY)
    if (m_bIsFixedFunction) {
        painter->updateFixedFunction
            (updates & (QGLPainter::UpdateColor |
                        QGLPainter::UpdateMatrices));
        return;
    }
#endif
    if (!m_pShaderProgram)
        return;
    if ((updates & QGLPainter::UpdateColor) != 0) {
//        if (painter->isPicking())
//            m_pShaderProgram->setUniformValue(m_iColorUniform, painter->pickColor());
//        else
            m_pShaderProgram->setUniformValue(m_iColorUniform, painter->color());
    }
    if ((updates & QGLPainter::UpdateMatrices) != 0) {
        QMatrix4x4 proj = painter->projectionMatrix();
        QMatrix4x4 mv = painter->modelViewMatrix();
        m_pShaderProgram->setUniformValue(m_iMatrixUniform, proj * mv);
    }
#endif
}

VIEW_END_NAMESPACE
