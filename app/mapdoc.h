/*! \file mapdoc.h
 * \brief The main map document class.
 *
 * The MapDoc class details any information that will be serialised when a map source file is saved.
 * This includes geometry and entities but also settings such as camera positions, grid size, snapping, etc.
 * Geometry and entities are kept inside an octree that represents the entire 3D space in the level.
 */

#ifndef MAPDOC_H
#define MAPDOC_H

#include <QObject>
#include "octree/octree.h"

#define MAPDOC_VERSION 10

class MapDoc : public QObject
{
    Q_OBJECT
public:
    inline static int getVersion() { return MAPDOC_VERSION; }

    explicit MapDoc(QObject *parent = 0);
    
signals:
    
public slots:
    
private:

};

#endif // MAPDOC_H
