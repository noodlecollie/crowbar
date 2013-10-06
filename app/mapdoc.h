/*! \file mapdoc.h
 * \brief The main map document class.
 *
 * The MapDoc class details any information that will be serialised when a map source file is saved.
 * This includes geometry and entities but also settings such as camera positions, grid size, snapping, etc.
 * Geometry and entities are kept inside an octree that represents the entire 3D space in the level.
 *
 * \note Simon Perreault's octree's documentation can be found at http://nomis80.org/code/doc/classOctree.html
 */

#ifndef MAPDOC_H
#define MAPDOC_H

#include <QObject>
#include "octree/octree.h"

//! \def MAPDOC_VERSION
//! \brief Current version of the mapdoc: [major][minor].
#define MAPDOC_VERSION 10

/**
 * @brief The MapDoc class
 *
 * The MapDoc class details any information that will be serialised when a map source file is saved.
 * This includes geometry and entities but also settings such as camera positions, grid size, snapping, etc.
 * Geometry and entities are kept inside an octree that represents the entire 3D space in the level.
 */
class MapDoc : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Gets the current version of the mapdoc.
     * @return Mapdoc version.
     */
    inline static int getVersion() { return MAPDOC_VERSION; }

    /**
     * @brief Default constructor.
     * @param parent Parent object.
     */
    explicit MapDoc(QObject *parent = 0);
    
    virtual ~MapDoc() {}
    
signals:
    
public slots:
    
private:

};

#endif // MAPDOC_H
