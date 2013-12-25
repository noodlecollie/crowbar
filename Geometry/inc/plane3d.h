#ifndef PLANE3D_H
#define PLANE3D_H

#include <QObject>
#include <QVector3D>

class Plane3D : public QObject
{
    Q_OBJECT
public:
    explicit Plane3D(QObject *parent = 0);
    
signals:
    
public slots:
    
private:
    QVector3D   m_vecNormal;
    QVector3D   m_vecPoint;
};

#endif // PLANE3D_H
