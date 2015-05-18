#ifndef MODELTREEA_H
#define MODELTREEA_H

#include <QAbstractItemModel>

class ModelTreeA : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ModelTreeA(QObject *parent = 0);
    ~ModelTreeA();

    virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    virtual int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual QModelIndex parent(const QModelIndex &child) const Q_DECL_OVERRIDE;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setRoot(QObject* obj);

signals:

public slots:

private:
    QObject* getItem(const QModelIndex &index) const;
    static bool isValidForParentIndex(const QObject* obj, int row);
    static int rowToChildIndex(const QObject* parent, int row);
    static int sumOfPropertiesAndChildren(const QObject* obj);
    static QString propertyName(const QObject *obj, int propertyIndex);
    static QVariant propertyValue(const QObject* obj, int propertyIndex);

    QObject* m_pRootItem;
};

#endif // MODELTREEA_H
