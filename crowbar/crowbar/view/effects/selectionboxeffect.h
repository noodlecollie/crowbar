#ifndef SELECTIONBOXEFFECT_H
#define SELECTIONBOXEFFECT_H

#include <QGLAbstractEffect>
#include "view_global.h"

VIEW_BEGIN_NAMESPACE

class VIEWSHARED_EXPORT SelectionBoxEffect : public QGLAbstractEffect
{
public:
    SelectionBoxEffect();
    virtual ~SelectionBoxEffect();

    virtual bool supportsPicking() const;
    virtual void setActive(QGLPainter *painter, bool flag);
    virtual void update(QGLPainter *painter, QGLPainter::Updates updates);
    
private:
    QOpenGLShaderProgram*   m_pShaderProgram;
    int                     m_iMatrixUniform;
    int                     m_iColorUniform;
    bool                    m_bIsFixedFunction;
};

VIEW_END_NAMESPACE

#endif // SELECTIONBOXEFFECT_H
