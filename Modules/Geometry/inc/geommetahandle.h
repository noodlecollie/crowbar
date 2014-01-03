#ifndef GEOMMETAHANDLE_H
#define GEOMMETAHANDLE_H

#include "geometry_global.h"

#include <QObject>
#include <Qt3D/QBox3D>

GEOMETRY_BEGIN_NAMESPACE

typedef unsigned long GEOMHANDLE;
static const GEOMHANDLE INVALID_HANDLE = 0;

class GEOMETRYSHARED_EXPORT GeomMetaHandle : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(bool selected READ selected WRITE setSelected)
    Q_PROPERTY(GEOMHANDLE id READ id WRITE setId)
public:
    explicit GeomMetaHandle(QObject *parent = 0);
    
    bool selected() const;
    void setSelected(bool b);
    
    GEOMHANDLE id() const;
    void setId(GEOMHANDLE id);
    
    // Returns whether a given piece of geometry is valid.
    virtual bool isValid() const = 0;
    
    // Returns the bounding box of a given piece of geometry.
    virtual QBox3D boundingBox() const = 0;
    
    // Returns the centre of a given piece of geometry.
    virtual QVector3D center() const = 0;
    
signals:
    
public slots:
    
private:
    bool        m_bSelected;
    GEOMHANDLE  m_hID;
};

GEOMETRY_END_NAMESPACE

#endif // GEOMMETAHANDLE_H
