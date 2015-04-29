#ifndef DATABASE_H
#define DATABASE_H

#include "model_global.h"
#include <QObject>
#include <QAbstractItemModel>

MODEL_BEGIN_NAMESPACE

class DatabaseNode;

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
    explicit Database(QObject *parent = 0);
    ~Database();

    // QAbstractItemModel interface:
    virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    virtual int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual QModelIndex parent(const QModelIndex &child) const Q_DECL_OVERRIDE;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) Q_DECL_OVERRIDE;
    virtual bool insertColumns(int column, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;
    virtual bool removeColumns(int column, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;
    virtual bool insertRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;
    virtual bool removeRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;

signals:

public slots:

private:
    DatabaseNode* getNode(const QModelIndex &index) const;

    DatabaseNode*   m_pRootItem;
    QVariant        m_vHeaderKey;
    QVariant        m_vHeaderValue;
};

MODEL_END_NAMESPACE

#endif // DATABASE_H
