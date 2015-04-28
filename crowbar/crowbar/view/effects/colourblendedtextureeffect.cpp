#include "colourblendedtextureeffect.h"
#include <QString>
#include <QOpenGLShaderProgram>
#include "effectglobals.h"
#include <QtDebug>

//#define SHADER_DEBUG

VIEW_BEGIN_NAMESPACE

#if !defined(QGL_FIXED_FUNCTION_ONLY)

static char const debugVertexShader[] =
    "attribute highp vec4 vertex;\n"
    "attribute mediump vec4 color;\n"
    "uniform highp mat4 matrix;\n"
    "varying mediump vec4 qColor;\n"
    "void main(void)\n"
    "{\n"
    "    gl_Position = matrix * vertex;\n"
    "    qColor = color;\n"
    "}\n";

static char const debugFragmentShader[] =
    "varying mediump vec4 qColor;\n"
    "void main(void)\n"
    "{\n"
    "    gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);\n"
    "}\n";

// This is taken from litTextureVertexShader
// but changed to take into account the colour of each vertex.
static char const colourBlendedVertexShader[] =
    "attribute highp vec4 vertex;\n"
    "attribute highp vec3 normal;\n"
    "attribute highp vec4 texcoord;\n"
        
    // We need to add this so that it is recognised by the shader.
    "attribute mediump vec4 color;\n"
        
    "uniform highp mat4 matrix;\n"
    "uniform highp mat4 modelView;\n"
    "uniform highp mat3 normalMatrix;\n"
    "varying highp vec4 qt_TexCoord0;\n"
    "void main(void)\n"
    "{\n"
    "    gl_Position = matrix * vertex;\n"
    "    highp vec4 tvertex = modelView * vertex;\n"
    "    highp vec3 norm = normalize(normalMatrix * normal);\n"
    "    qLightVertex(tvertex, norm);\n"
    "    qt_TexCoord0 = texcoord;\n"
        
    // Blend the vertex colour with the colour calculated by qLightVertex().
    // This means we modify qColor - qSecondaryColor is used to hold the specular component.
    "    qColor *= color;\n"
        
    "}\n";

static char const colourBlendedFragmentShader[] =
    "uniform sampler2D tex;\n"
#if defined(QT_OPENGL_ES)
    "varying mediump vec4 qCombinedColor;\n"
#else
    "uniform bool separateSpecular;\n"
    "varying mediump vec4 qColor;\n"
    "varying mediump vec4 qSecondaryColor;\n"
#endif
    "varying highp vec4 qt_TexCoord0;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "    mediump vec4 col = texture2D(tex, qt_TexCoord0.st);\n"  
#if defined(QT_OPENGL_ES)
    "    gl_FragColor = col * qCombinedColor;\n"
#else
    "    if (separateSpecular) {\n"
    "        gl_FragColor = clamp(col * qColor + vec4(qSecondaryColor.xyz, 0.0), 0.0, 1.0);\n"
    "    } else {\n"
    "        mediump vec4 lcolor = clamp(qColor + vec4(qSecondaryColor.xyz, 0.0), 0.0, 1.0);\n"
    "        gl_FragColor = col * lcolor;\n"
    "    }\n"
#endif    
    "}\n";

#endif  // !defined(QGL_FIXED_FUNCTION_ONLY)

#ifndef SHADER_DEBUG

#ifndef GL_MODULATE
#define GL_MODULATE 0x2100
#endif

ColourBlendedTextureEffect::ColourBlendedTextureEffect() : QGLAbstractEffect()
{
#if defined(QGL_FIXED_FUNCTION_ONLY)
    init(GL_MODULATE, 0, 0, QString());
#else
    init(GL_MODULATE, colourBlendedVertexShader, colourBlendedFragmentShader, QLatin1String("crowbar.texture.colourblended"));
#endif
}

ColourBlendedTextureEffect::~ColourBlendedTextureEffect()
{
}

void ColourBlendedTextureEffect::init(GLenum mode, const char *vshader, const char *fshader, const QString &programName)
{
    m_iTextureMode = mode;
    m_pVertexShader = vshader;
    m_pFragmentShader = fshader;
    m_szProgramName = programName;
}

void ColourBlendedTextureEffect::setActive(QGLPainter *painter, bool flag)
{
#if defined(QGL_FIXED_FUNCTION_ONLY)
    Q_UNUSED(painter);
    Q_D(QGLLitMaterialEffect);
    if (flag) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        if (m_iTextureMode) {
            qt_gl_ClientActiveTexture(GL_TEXTURE0);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_iTextureMode);
            glEnable(GL_TEXTURE_2D);
        }
    } else {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        if (m_iTextureMode) {
            qt_gl_ClientActiveTexture(GL_TEXTURE0);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisable(GL_TEXTURE_2D);
        }
    }
#else
    Q_UNUSED(painter);
#if !defined(QGL_SHADERS_ONLY)
    if (painter->isFixedFunction()) {
        m_bIsFixedFunction = true;
        if (flag) {
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            if (m_iTextureMode) {
                qt_gl_ClientActiveTexture(GL_TEXTURE0);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_iTextureMode);
                glEnable(GL_TEXTURE_2D);
            }
        } else {
            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT0);
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
            glDisableClientState(GL_COLOR_ARRAY);
            if (m_iTextureMode) {
                qt_gl_ClientActiveTexture(GL_TEXTURE0);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                glDisable(GL_TEXTURE_2D);
            }
        }
        return;
    }
#endif
    QOpenGLShaderProgram *program = painter->cachedProgram(m_szProgramName);
    m_pProgram = program;
    if (!program) {
        if (!flag)
            return;
        program = new QOpenGLShaderProgram;
        program->addShaderFromSourceCode(QOpenGLShader::Vertex, createVertexSource(litMaterialLightingShader, m_pVertexShader));
        program->addShaderFromSourceCode(QOpenGLShader::Fragment, m_pFragmentShader);
        program->bindAttributeLocation("vertex", QGL::Position);
        program->bindAttributeLocation("normal", QGL::Normal);
        program->bindAttributeLocation("color", QGL::Color);
        if (m_iTextureMode != 0)
            program->bindAttributeLocation("texcoord", QGL::TextureCoord0);
        if (!program->link()) {
            qWarning("QGLLitMaterialEffect::setActive(): could not link shader program");
            delete program;
            program = 0;
            return;
        }
        painter->setCachedProgram(m_szProgramName, program);
        m_pProgram = program;
        m_iMatrixUniform = program->uniformLocation("matrix");
        m_iModelViewUniform = program->uniformLocation("modelView");
        m_iNormalMatrixUniform = program->uniformLocation("normalMatrix");
        program->bind();
        if (m_iTextureMode != 0) {
            program->setUniformValue("tex", 0);
            program->enableAttributeArray(QGL::TextureCoord0);
        }
        program->enableAttributeArray(QGL::Position);
        program->enableAttributeArray(QGL::Normal);
        program->enableAttributeArray(QGL::Color);
    } else if (flag) {
        m_iMatrixUniform = program->uniformLocation("matrix");
        m_iModelViewUniform = program->uniformLocation("modelView");
        m_iNormalMatrixUniform = program->uniformLocation("normalMatrix");
        program->bind();
        if (m_iTextureMode != 0) {
            program->setUniformValue("tex", 0);
            program->enableAttributeArray(QGL::TextureCoord0);
        }
        program->enableAttributeArray(QGL::Position);
        program->enableAttributeArray(QGL::Normal);
        program->enableAttributeArray(QGL::Color);
    } else {
        program->disableAttributeArray(QGL::Position);
        program->disableAttributeArray(QGL::Normal);
        program->enableAttributeArray(QGL::Color);
        if (m_iTextureMode != 0)
            program->disableAttributeArray(QGL::TextureCoord0);
        program->release();
    }
#endif
}

void ColourBlendedTextureEffect::update
        (QGLPainter *painter, QGLPainter::Updates updates)
{
#if defined(QGL_FIXED_FUNCTION_ONLY)
    painter->updateFixedFunction
        (updates & (QGLPainter::UpdateMatrices |
                    QGLPainter::UpdateLights |
                    QGLPainter::UpdateMaterials));
#else
#if !defined(QGL_SHADERS_ONLY)
    if (m_bIsFixedFunction) {
        painter->updateFixedFunction
            (updates & (QGLPainter::UpdateMatrices |
                        QGLPainter::UpdateLights |
                        QGLPainter::UpdateMaterials));
        return;
    }
#endif
    QOpenGLShaderProgram *program = m_pProgram;
    if (!program)
        return;
    if ((updates & QGLPainter::UpdateMatrices) != 0) {
        program->setUniformValue(m_iMatrixUniform, painter->combinedMatrix());
        program->setUniformValue(m_iModelViewUniform, painter->modelViewMatrix());
        program->setUniformValue(m_iNormalMatrixUniform, painter->normalMatrix());
    }
    const QGLLightParameters *lparams = painter->mainLight();
    QMatrix4x4 ltransform = painter->mainLightTransform();
    const QGLLightModel *model = painter->lightModel();
    if ((updates & (QGLPainter::UpdateLights | QGLPainter::UpdateMaterials)) != 0) {
        // Set the uniform variables for the light.
        program->setUniformValue
            ("sdli", lparams->eyeSpotDirection(ltransform).normalized());
        QVector4D pli = lparams->eyePosition(ltransform);
        program->setUniformValue("pli", QVector3D(pli.x(), pli.y(), pli.z()));
        program->setUniformValue("pliw", GLfloat(pli.w()));
        program->setUniformValue("srli", GLfloat(lparams->spotExponent()));
        program->setUniformValue("crli", GLfloat(lparams->spotAngle()));
        program->setUniformValue("ccrli", GLfloat(lparams->spotCosAngle()));
#if !defined(QT_OPENGL_ES)
        // Attenuation is not supported under ES, for performance.
        program->setUniformValue("k0", GLfloat(lparams->constantAttenuation()));
        program->setUniformValue("k1", GLfloat(lparams->linearAttenuation()));
        program->setUniformValue("k2", GLfloat(lparams->quadraticAttenuation()));
#endif

        // Set the uniform variables for the light model.
#if !defined(QT_OPENGL_ES)
        program->setUniformValue("twoSided", (int)(model->model() == QGLLightModel::TwoSided));
#endif
        program->setUniformValue("viewerAtInfinity", (int)(model->viewerPosition() == QGLLightModel::ViewerAtInfinity));
#if !defined(QT_OPENGL_ES)
        if (m_iTextureMode != 0)
            program->setUniformValue("separateSpecular", (int)(model->colorControl() == QGLLightModel::SeparateSpecularColor));
#endif

        // Set the uniform variables for the front and back materials.
#if defined(QT_OPENGL_ES)
        static const int MaxMaterials = 1;
#else
        static const int MaxMaterials = 2;
#endif
        QVector4D acm[MaxMaterials];
        QVector4D dcm[MaxMaterials];
        QVector4D scm[MaxMaterials];
        QVector4D ecm[MaxMaterials];
        float srm[MaxMaterials];
        const QGLMaterial *mparams = painter->faceMaterial(QGL::FrontFaces);
        acm[0] = colorToVector4(mparams->ambientColor(), lparams->ambientColor());
        dcm[0] = colorToVector4(mparams->diffuseColor(), lparams->diffuseColor());
        scm[0] = colorToVector4(mparams->specularColor(), lparams->specularColor());
        ecm[0] = colorToVector4(mparams->emittedLight()) +
                 colorToVector4(mparams->ambientColor(),
                                model->ambientSceneColor());
        srm[0] = (float)(mparams->shininess());
#if !defined(QT_OPENGL_ES)
        mparams = painter->faceMaterial(QGL::BackFaces);
        acm[1] = colorToVector4(mparams->ambientColor(), lparams->ambientColor());
        dcm[1] = colorToVector4(mparams->diffuseColor(), lparams->diffuseColor());
        scm[1] = colorToVector4(mparams->specularColor(), lparams->specularColor());
        ecm[1] = colorToVector4(mparams->emittedLight()) +
                 colorToVector4(mparams->ambientColor(),
                                model->ambientSceneColor());
        srm[1] = (float)(mparams->shininess());
#endif
        program->setUniformValueArray("acm", (const GLfloat *)acm, MaxMaterials, 4);
        program->setUniformValueArray("dcm", (const GLfloat *)dcm, MaxMaterials, 4);
        program->setUniformValueArray("scm", (const GLfloat *)scm, MaxMaterials, 4);
        program->setUniformValueArray("ecm", (const GLfloat *)ecm, MaxMaterials, 4);
        program->setUniformValueArray("srm", srm, MaxMaterials, 1);
    }
#endif
}

#else // SHADER_DEBUG

ColourBlendedTextureEffect::ColourBlendedTextureEffect()
#if defined(QGL_FIXED_FUNCTION_ONLY)
    : ColourBlendedTextureEffect(GL_MODULATE, 0, 0, QString())
#else
    : ColourBlendedTextureEffect(GL_MODULATE,
                          debugVertexShader, debugFragmentShader,
                          QLatin1String("crowbar.texture.colourblended"))
#endif
{
}

ColourBlendedTextureEffect::~ColourBlendedTextureEffect()
{
}

ColourBlendedTextureEffect::ColourBlendedTextureEffect(GLenum mode, const char *vshader, const char *fshader, const QString &programName)
    : QGLAbstractEffect()
{
    m_iTextureMode = mode;
    m_pVertexShader = vshader;
    m_pFragmentShader = fshader;
    m_szProgramName = programName;
}

void ColourBlendedTextureEffect::setActive(QGLPainter *painter, bool flag)
{
#if defined(QGL_FIXED_FUNCTION_ONLY)
    Q_UNUSED(painter);
    if (flag) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
    } else {
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }
#else
    Q_UNUSED(painter);
#if !defined(QGL_SHADERS_ONLY)
    if (painter->isFixedFunction()) {
        m_bIsFixedFunction = true;
        if (flag) {
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
        } else {
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_COLOR_ARRAY);
        }
        return;
    }
#endif
    QOpenGLShaderProgram *program =
        painter->cachedProgram(m_szProgramName);
    m_pProgram = program;
    if (!program) {
        if (!flag)
            return;
        program = new QOpenGLShaderProgram();
        program->addShaderFromSourceCode(QOpenGLShader::Vertex, m_pVertexShader);
        program->addShaderFromSourceCode(QOpenGLShader::Fragment, m_pFragmentShader);
        program->bindAttributeLocation("vertex", QGL::Position);
        program->bindAttributeLocation("color", QGL::Color);
        if (!program->link()) {
            qWarning("QGLPerVertexColorEffect::setActive(): could not link shader program");
            delete program;
            program = 0;
            return;
        }
        painter->setCachedProgram(QLatin1String("qt.color.pervertex"), program);
        m_pProgram = program;
        m_iMatrixUniform = program->uniformLocation("matrix");
        program->bind();
        program->enableAttributeArray(QGL::Position);
        program->enableAttributeArray(QGL::Color);
    } else if (flag) {
        m_iMatrixUniform = program->uniformLocation("matrix");
        program->bind();
        program->enableAttributeArray(QGL::Position);
        program->enableAttributeArray(QGL::Color);
    } else {
        program->disableAttributeArray(QGL::Position);
        program->disableAttributeArray(QGL::Color);
        program->release();
    }
#endif
}

void ColourBlendedTextureEffect::update
        (QGLPainter *painter, QGLPainter::Updates updates)
{
#if defined(QGL_FIXED_FUNCTION_ONLY)
    painter->updateFixedFunction(updates & QGLPainter::UpdateMatrices);
#else
    Q_UNUSED(painter);
#if !defined(QGL_SHADERS_ONLY)
    if (m_bIsFixedFunction) {
        painter->updateFixedFunction(updates & QGLPainter::UpdateMatrices);
        return;
    }
#endif
    if (!m_pProgram)
        return;
    if ((updates & QGLPainter::UpdateMatrices) != 0) {
        m_pProgram->setUniformValue
            (m_iMatrixUniform, painter->combinedMatrix());
    }
#endif
}

#endif // SHADER_DEBUG

VIEW_END_NAMESPACE
