#ifndef QMLCLASS_H
#define QMLCLASS_H

#include <QObject>
#include <QQmlListProperty>
#include <QVector>

class QMLClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QObject> props READ props)
public:
    explicit QMLClass(QObject *parent = 0);
    
    QQmlListProperty<QObject> props();
    
    void appendProp(QObject* obj);
    void clearProps();
    QObject* propAt(int index) const;
    int propCount() const;
    
signals:
    
public slots:
    
private:
    QVector<QObject*> m_PropVector;
};

#endif // QMLCLASS_H
