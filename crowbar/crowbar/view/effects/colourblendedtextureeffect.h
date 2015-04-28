#ifndef COLOURBLENDEDTEXTUREEFFECT_H
#define COLOURBLENDEDTEXTUREEFFECT_H

#include "view_global.h"
#include <QGLAbstractEffect>

VIEW_BEGIN_NAMESPACE

class ColourBlendedTextureEffect : public QGLAbstractEffect
{
public:
    ColourBlendedTextureEffect();
    virtual ~ColourBlendedTextureEffect();
    
    virtual bool supportsPicking() const { return false; }
    virtual void setActive(QGLPainter *painter, bool flag);
    virtual void update(QGLPainter *painter, QGLPainter::Updates updates);
    
private:
    void init(GLenum mode, const char *vshader, const char *fshader, const QString& programName);

    QOpenGLShaderProgram *m_pProgram;
    int m_iMatrixUniform;
    int m_iModelViewUniform;
    int m_iNormalMatrixUniform;
    GLenum m_iTextureMode;
    const char *m_pVertexShader;
    const char *m_pFragmentShader;
    QString m_szProgramName;
    bool m_bIsFixedFunction;
};

VIEW_END_NAMESPACE

#endif // COLOURBLENDEDTEXTUREEFFECT_H
