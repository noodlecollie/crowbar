#include "database.h"
#include "databaserow.h"

MODEL_BEGIN_NAMESPACE

Database::Database(QObject *parent) : QAbstractItemModel(parent)
{

}

Database::~Database()
{

}

QVariant Database::data(const QModelIndex &index, int role) const
{
    // TODO: Incomplete.
    return QVariant();
}

QVariant Database::headerData(int section, Qt::Orientation orientation, int role) const
{
    // TODO: Incomplete.
    return QVariant("Placeholder");
}

int Database::rowCount(const QModelIndex &parent) const
{
    // TODO: Incomplete.
    return 0;
}

int Database::columnCount(const QModelIndex &parent) const
{
    // TODO: Incomplete.
    return 1;
}

QModelIndex Database::index(int row, int column, const QModelIndex &parent) const
{
    // TODO: Incomplete.
    return QModelIndex();
}

QModelIndex Database::parent(const QModelIndex &child) const
{
    // TODO: Incomplete.
    return QModelIndex();
}

Qt::ItemFlags Database::flags(const QModelIndex &index) const
{
    // TODO: Incomplete.
    return Qt::NoItemFlags;
}

MODEL_END_NAMESPACE
