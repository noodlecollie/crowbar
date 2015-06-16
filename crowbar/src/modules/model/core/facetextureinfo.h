#ifndef FACETEXTUREINFO_H
#define FACETEXTUREINFO_H

#include "model_global.h"
#include "changenotifier.h"
#include <QVector2D>

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT FaceTextureInfo : public ChangeNotifier
{
    Q_OBJECT
    Q_PROPERTY(QVector2D translation READ translation WRITE setTranslation NOTIFY translationChanged)
public:
    explicit FaceTextureInfo(QObject *parent = 0);

    QVector2D translation() const;
    void setTranslation(const QVector2D &vec);

signals:
    void translationChanged(const QVector2D&);

public slots:

private:
    QVector2D m_vecTranslation;
};

MODEL_END_NAMESPACE

#endif // FACETEXTUREINFO_H
