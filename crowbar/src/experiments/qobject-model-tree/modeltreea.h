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
    static inline bool isChildRow(const QObject* object, int row)
    {
        Q_ASSERT(object);
        row = childIndex(object, row);
        
        // Return whether it is in range of the number of children.
        return row >= 0 && row < object->children().count();
    }
    
    static inline int childIndex(const QObject* object, int row)
    {
        Q_ASSERT(object);
        // Subtract the number of properties from the row value.
        return row - object->metaObject()->propertyCount();
    }
    
    // Returns the QObject that holds the child or property referred to by the index,
    // or NULL if the index does not point to a QObject.
    static inline QObject* ownerObject(const QModelIndex &index)
    {
        return index.isValid() ? static_cast<QObject*>(index.internalPointer()) : NULL;
    }
    
    // Returns the QObject that the index refers to, if the index points to a child of the ownerObject(),
    // or NULL otherwise.
    QObject* childAt(const QModelIndex &index) const;
    
    static inline int totalRowCount(const QObject* object)
    {
        Q_ASSERT(object);
        return object->metaObject()->propertyCount() + object->children().count();
    }

    QObject* m_pRootItem;
};

#endif // MODELTREEA_H
