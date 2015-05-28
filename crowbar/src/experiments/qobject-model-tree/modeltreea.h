#ifndef MODELTREEA_H
#define MODELTREEA_H

#include <QAbstractItemModel>

/*
 * The new tree (not this one) should work on the following principles:
 * - Indices in the model refer to properties on QObject instances: the property value (column 0),
 *   name (column 1) or type (column 2).
 * - Properties can be lists or single values.
 * - If a property is a single value, the index referring to the property (column 0) has no child rows.
 * - If a property is a list, the index referring to the property (column 0) has a child row for each list item.
 * - Property values (including list items) are reference or value types. If it a value type, the value is copied into
 *   the property and its index has no child rows. If the value is a reference type, it refers to another QObject that
 *   is owned by the current QObject and whatever properties the object has are added as child rows of the index.
 *   If the value is a reference to a non-owned QObject then it is should be specified as a QSharedPointer or the like,
 *   which itself is a value type.
 * - If an object is owned by another, it should be a QObject child of the parent.
 * 
 * We can prototype this in pseudo-JSON:
 * 
 *  // Child rows are the available properties.
 *  {
 *      property: int {}            // No child rows
 *      property: QSharedPointer {} // No child rows - reference to non-owned object is still a value type
 *      property: QObject*          // Child rows as object is owned
 *      {
 *          property: QString {}    // No child rows
 *          property: float {}      // No child rows
 *      }
 * 
 *      property: QList<int>    // Child rows correspond to number of items in list
 *      {
 *          int,    // No child rows for items since they are value types
 *          int,
 *          int
 *      }
 * 
 *      property: QList<QPointer<QObject>>  // Child rows correspond to number of items in list
 *      {
 *          QPointer<QObject>,  // No child rows - reference to non-owned object is still a value type
 *          QPointer<QObject>,
 *          QPointer<QObject>
 *      }
 * 
 *      property: QList<QObject*>   // Child rows correspond to number of items in list
 *      {
 *          QObject* {...}, // Child rows as objects are owned
 *          QObject* {...},
 *          QObject* {...}
 *      }
 *  }
 * 
 * A brush might be specified like this:
 * {
 *      // A list of brushes.
 *      property: (Qlist<BrushObject*>) brushList
 *      {
 *          BrushObject*    // A brush in the list.
 *          {
 *              // A list of vertices owned by the brush.
 *              property: (QList<VertexObject*>) vertices
 *              {
 *                  VertexObject* {...},    // Some vertices.
 *                  VertexObject* {...},
 *                  ...
 *              }
 * 
 *              // A list of edges owned by the brush.
 *              property: (QList<EdgeObject*>) edges
 *              {
 *                  // A sample edge object.
 *                  EdgeObject*
 *                  {
 *                      // The vertices are not owned, so the properties contain value references.
 *                      property: (QPointer<VertexObject>) vertex0
 *                      property: (QPointer<VertexObject>) vertex1
 *                  },
 *                  
 *                  ... // Other, similar edges
 *              }
 * 
 *              // A list of faces owned by the brush.
 *              property: (QList<FaceObject*>) faces
 *              {
 *                  // A sample face object
 *                  FaceObject*
 *                  {
 *                      // The vertices are not owned, so each item is just a value reference.
 *                      property: (QList<QPointer<VertexObject>>) vertices
 *                      {
 *                          QPointer<VertexObject>,
 *                          ...
 *                      }
 *                  }
 *              }
 *          },
 * 
 *          ... // Other, similar brushes
 *      }
 * }
 */

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

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

    void setRoot(QObject* obj);
    
    bool ownsRootItem() const;
    void setOwnsRootItem(bool owns);
    
    bool designablePropertiesOnly() const;
    void setDesignablePropertiesOnly(bool enabled);

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
    bool m_bOwnsRoot;
    bool m_bDesignablePropertiesOnly;
};

#endif // MODELTREEA_H
