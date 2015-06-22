#ifndef BRUSHVERTEX_H
#define BRUSHVERTEX_H

#include "model_global.h"
#include <QObject>
#include <QVector3D>

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT BrushVertex : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
public:
    explicit BrushVertex(QObject *parent = 0);

    QVector3D position() const;
    void setPosition(const QVector3D &pos);

signals:
    void positionChanged(const QVector3D&);

public slots:

private:
    QVector3D m_vecPosition;
};

MODEL_END_NAMESPACE

#endif // BRUSHVERTEX_H
