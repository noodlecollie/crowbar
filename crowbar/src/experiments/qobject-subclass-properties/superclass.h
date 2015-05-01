#ifndef SUPERCLASS_H
#define SUPERCLASS_H

#include <QObject>

class Superclass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int integerProperty READ integerProperty WRITE setIntegerProperty NOTIFY integerPropertyChanged)
    Q_PROPERTY(bool booleanProperty READ booleanProperty WRITE setBooleanProperty NOTIFY booleanPropertyChanged)
public:
    explicit Superclass(QObject* parent = 0);
    ~Superclass();
    
    int integerProperty() const { return m_iIntegerProperty; }
    void setIntegerProperty(int value) { m_iIntegerProperty = value; emit integerPropertyChanged(m_iIntegerProperty); }
    
    int booleanProperty() const { return m_bBooleanProperty; }
    void setBooleanProperty(int value) { m_bBooleanProperty = value; emit booleanPropertyChanged(m_bBooleanProperty); }
    
signals:
    void integerPropertyChanged(int);
    void booleanPropertyChanged(bool);
    
private:
    int m_iIntegerProperty;
    bool m_bBooleanProperty;
};

#endif // SUPERCLASS_H
