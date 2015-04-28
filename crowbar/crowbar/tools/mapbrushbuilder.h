#ifndef MAPBRUSHBUILDER_H
#define MAPBRUSHBUILDER_H

#include <QObject>
#include "tools_global.h"
#include "model_global.h"
#include <QBox3D>

class QGLTexture2D;

MODEL_BEGIN_NAMESPACE
class MapBrush;
MODEL_END_NAMESPACE

TOOLS_BEGIN_NAMESPACE

class TOOLSSHARED_EXPORT MapBrushBuilder : public QObject
{
    Q_OBJECT
public:
    explicit MapBrushBuilder(QObject *parent = 0);

    // Vertices are defined relative to the origin.
    static MODEL_NAMESPACE::MapBrush* axisAlignedBrush(const QVector3D &min, const QVector3D &max, QGLTexture2D* texture);
    
    // Origin is set to be the centre of the bounds.
    static MODEL_NAMESPACE::MapBrush* axisAlignedBrush(const QBox3D &bounds, QGLTexture2D* texture);

signals:

public slots:

};

TOOLS_END_NAMESPACE

#endif // MAPBRUSHBUILDER_H
