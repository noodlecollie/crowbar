#include "facetextureinfo.h"

MODEL_BEGIN_NAMESPACE

FaceTextureInfo::FaceTextureInfo(QObject *parent) : ChangeNotifier(parent)
{

}

QVector2D FaceTextureInfo::translation() const
{
    return m_vecTranslation;
}

void FaceTextureInfo::setTranslation(const QVector2D &vec)
{
    if ( vec == m_vecTranslation ) return;

    m_vecTranslation = vec;
    emit translationChanged(m_vecTranslation);
    generateChange(ChangeNotifier::FlagRender);
}

MODEL_END_NAMESPACE
