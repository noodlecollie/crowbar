#ifndef DATABASE_H
#define DATABASE_H

#include "model_global.h"
#include <QObject>
#include <QAbstractItemModel>

MODEL_BEGIN_NAMESPACE

class DatabaseItem;

/*!
 * \brief The Database class holds the core information used in a map.
 *
 * Any data within a map is held within the database class. The database is structured in a hierarchy according to the type
 * of information being stored - information of the same type is grouped together for convenience and efficiency.
 * In and above this, the database provides no other structure over the data - this is the job of the DataView classes.
 */
class MODELSHARED_EXPORT Database : public QAbstractItemModel
{
    Q_OBJECT
    Q_ENUMS(DatabaseDataRole)
public:
    enum DatabaseDataRole
    {
        ThreeDimensionalRole = Qt::UserRole    //! Data specifically relating to the 3D representation of the item.
    };

    explicit Database(QObject *parent = 0);
    ~Database();

    // QAbstractItemModel interface:
    virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    virtual int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual QModelIndex parent(const QModelIndex &child) const Q_DECL_OVERRIDE;

signals:

public slots:

private:
    DatabaseItem*   m_pRootItem;
};

MODEL_END_NAMESPACE

#endif // DATABASE_H
