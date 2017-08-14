#include "profileritemmodeladatper.h"
#include "profilermodel.h"

namespace Profiling
{
    ProfilerItemModelAdatper::ProfilerItemModelAdatper(const ProfilerModel &model, QObject *parent)
        : QAbstractItemModel(parent),
          m_Model(model)
    {

    }

    void ProfilerItemModelAdatper::refresh()
    {
        beginResetModel();

        m_ChildLists.clear();
        m_ChildLists.reserve(m_Model.dataSlotCount());

        for ( quint32 i = 0; i < m_Model.dataSlotCount(); ++i )
        {
            m_ChildLists.append(m_Model.children(i));
        }

        endResetModel();
    }

    QModelIndex ProfilerItemModelAdatper::index(int row, int column, const QModelIndex &parent) const
    {
        if ( column < 0 || column >= TOTAL_COLUMNS )
        {
            return QModelIndex();
        }

        const int slotIndex = static_cast<const int>(parent.internalId());

        if ( !isValidSlot(slotIndex) )
        {
            return QModelIndex();
        }

        Q_ASSERT_X(m_ChildLists.count() > slotIndex, Q_FUNC_INFO, "Expected a child list to exist!");

        const SlotVector& childrenOfParent = m_ChildLists[slotIndex];
        if ( row < 0 || row >= childrenOfParent.count() )
        {
            return QModelIndex();
        }

        return createIndex(row, column, children.at(row));
    }

    QModelIndex ProfilerItemModelAdatper::parent(const QModelIndex &child) const
    {
        const int slotIndex = static_cast<const int>(child.internalId());

        if ( !isValidSlot(slotIndex) )
        {
            return QModelIndex();
        }


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

    bool ProfilerItemModelAdatper::isValidSlot(int slotIndex) const
    {
        return slotIndex != ProfilerModel::NULL_SLOT_ID && (slotindex < 0 || slotIndex >= m_Model.dataSlotCount());
    }
}
