#include "profileritemmodeladatper.h"
#include "profilermodel.h"

ProfilerItemModelAdatper::ProfilerItemModelAdatper(const ProfilerModel &model, QObject *parent)
    : QAbstractItemModel(parent),
      m_Model(model)
{

}

QModelIndex ProfilerItemModelAdatper::index(int row, int column, const QModelIndex &parent) const
{
    // TODO
    return QModelIndex();
}

QModelIndex ProfilerItemModelAdatper::parent(const QModelIndex &child) const
{
    // TODO
    return QModelIndex();
}

int ProfilerItemModelAdatper::rowCount(const QModelIndex &parent) const
{
    // TODO
    return 0;
}

int ProfilerItemModelAdatper::columnCount(const QModelIndex &parent) const
{
    // TODO
    return 0;
}

QVariant ProfilerItemModelAdatper::data(const QModelIndex &index, int role) const
{
    // TODO
    return QVariant();
}
