#include "effectglobals.h"
#include <QOpenGLFunctions>

VIEW_BEGIN_NAMESPACE

QByteArray createVertexSource(const char *lighting, const char *extra)
{
    QByteArray contents(lighting);
    return contents + extra;
}

QVector4D colorToVector4(const QColor &color)
{
    return QVector4D(color.redF(), color.greenF(),
                     color.blueF(), color.alphaF());
}

QVector4D colorToVector4(const QColor &color, const QColor &lightColor)
{
    return QVector4D(color.redF() * lightColor.redF(),
                     color.greenF() * lightColor.greenF(),
                     color.blueF() * lightColor.blueF(),
                     color.alphaF() * lightColor.alphaF());
}

#if !defined(QT_OPENGL_ES)

typedef void (QOPENGLF_APIENTRYP q_PFNGLCLIENTACTIVETEXTUREPROC) (GLenum);

class QGLMultiTextureExtensions
{
public:
    QGLMultiTextureExtensions() :
        clientActiveTexture(0),
        multiTextureResolved(false)
    {
    }
    ~QGLMultiTextureExtensions();

    void invalidateExtension();

    q_PFNGLCLIENTACTIVETEXTUREPROC clientActiveTexture;
    bool multiTextureResolved;
};

static QGLMultiTextureExtensions *qt_multitexture_funcs = 0;

QGLMultiTextureExtensions::~QGLMultiTextureExtensions()
{
    qt_multitexture_funcs = 0;
}

void QGLMultiTextureExtensions::invalidateExtension()
{
    qt_multitexture_funcs = 0;
}

static QGLMultiTextureExtensions *resolveMultiTextureExtensions
    (const QOpenGLContext *ctx)
{
    Q_ASSERT(ctx);
    if (!qt_multitexture_funcs)
        qt_multitexture_funcs = new QGLMultiTextureExtensions();
    if (!(qt_multitexture_funcs->multiTextureResolved)) {
        qt_multitexture_funcs->multiTextureResolved = true;
        if (!qt_multitexture_funcs->clientActiveTexture) {
            QOpenGLContext *vctx = const_cast<QOpenGLContext*>(ctx);
            qt_multitexture_funcs->clientActiveTexture = (q_PFNGLCLIENTACTIVETEXTUREPROC)
                vctx->getProcAddress("glClientActiveTexture");
        }
        if (!qt_multitexture_funcs->clientActiveTexture) {
            QOpenGLContext *vctx = const_cast<QOpenGLContext*>(ctx);
            qt_multitexture_funcs->clientActiveTexture = (q_PFNGLCLIENTACTIVETEXTUREPROC)
                vctx->getProcAddress("glClientActiveTextureARB");
        }
    }
    return qt_multitexture_funcs;
}

void qt_gl_ClientActiveTexture(GLenum texture)
{
    const QOpenGLContext *ctx = QOpenGLContext::currentContext();
    if (!ctx)
        return;
    QGLMultiTextureExtensions *extn = resolveMultiTextureExtensions(ctx);
    if (extn->clientActiveTexture)
        extn->clientActiveTexture(texture);
}

#endif

VIEW_END_NAMESPACE
