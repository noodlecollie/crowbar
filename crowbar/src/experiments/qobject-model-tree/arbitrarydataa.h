#ifndef ARBITRARYDATAA_H
#define ARBITRARYDATAA_H

#include <QObject>
#include <QList>
#include <QVariant>

class ArbitraryDataA : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int integerProperty READ integerProperty WRITE setIntegerProperty NOTIFY integerPropertyChanged)
    Q_PROPERTY(QString stringProperty READ stringProperty WRITE setStringProperty NOTIFY stringPropertyChanged)
    Q_PROPERTY(QList<QVariant> variantListProperty READ variantListProperty)
public:
    explicit ArbitraryDataA(QObject *parent = 0);
    ~ArbitraryDataA();

    int integerProperty() const;
    void setIntegerProperty(int value);

    QString stringProperty() const;
    void setStringProperty(const QString &value);
    
    QList<QVariant> variantListProperty() const;

signals:
    void integerPropertyChanged(int);
    void stringPropertyChanged(const QString&);

public slots:

private:
    int m_iIntegerProperty;
    QString m_szStringProperty;
    QList<QVariant> m_VariantListProperty;
};

#endif // ARBITRARYDATAA_H
