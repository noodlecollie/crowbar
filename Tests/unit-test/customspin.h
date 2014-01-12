#ifndef CUSTOMSPIN_H
#define CUSTOMSPIN_H

#include <QObject>
#include <QSpinBox>

class CustomSpin : public QSpinBox
{
    Q_OBJECT
public:
    explicit CustomSpin(QWidget *parent = 0);
    
signals:
    
public slots:
    void increment() { setValue((value() % 60) + 1); }
};

#endif // CUSTOMSPIN_H
