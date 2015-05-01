#ifndef SUBCLASS_H
#define SUBCLASS_H

#include "superclass.h"

class Subclass : public Superclass
{
    Q_OBJECT
    Q_PROPERTY(QString stringProperty READ stringProperty WRITE setStringProperty NOTIFY stringPropertyChanged)
public:
    explicit Subclass(QObject* parent = 0);
    ~Subclass();
    
    QString stringProperty() const { return m_szStringProperty; }
    void setStringProperty(const QString &value) { m_szStringProperty = value; emit stringPropertyChanged(m_szStringProperty); }
    
signals:
    void stringPropertyChanged(const QString&);
    
private:
    QString m_szStringProperty;
};

#endif // SUBCLASS_H
