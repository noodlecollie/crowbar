#ifndef MAPORIGINMARKER_H
#define MAPORIGINMARKER_H

#include <QObject>
#include <QGLSceneNode>
#include "view_global.h"
#include "irender.h"

VIEW_BEGIN_NAMESPACE

class VIEWSHARED_EXPORT MapOriginMarker : public QObject
{
    Q_OBJECT
public:
    explicit MapOriginMarker(QObject *parent = 0);

    QGLSceneNode* sceneNode(IRender::RenderMode mode) const;

signals:

public slots:

private:
    static QGeometryData originData();
};

VIEW_END_NAMESPACE

#endif // MAPORIGINMARKER_H
