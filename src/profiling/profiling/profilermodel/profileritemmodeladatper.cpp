#include "profileritemmodeladatper.h"
#include "profilermodel.h"

namespace
{
    inline int toSlotIndex(const QModelIndex& modelIndex)
    {
        return modelIndex.isValid() ? static_cast<int>(modelIndex.internalId()) : -1;
    }
}

namespace Profiling
{
    ProfilerItemModelAdatper::ProfilerItemModelAdatper(const ProfilerModel &model, QObject *parent)
        : QAbstractItemModel(parent),
          m_Model(model)
    {
        // Set child lists to have at least one entry, as index 0 represents the null QModelIndex.
        // We always want to be able to query how many top-level children exist.
        m_ChildLists.append(SlotVector());
    }

    void ProfilerItemModelAdatper::refresh()
    {
        beginResetModel();
        m_Model.exportChildren(m_ChildLists);
        endResetModel();
    }

    QModelIndex ProfilerItemModelAdatper::index(int row, int column, const QModelIndex &parent) const
    {
        if ( column < 0 || column >= TOTAL_COLUMNS )
        {
            return QModelIndex();
        }

        const int parentSlotIndex = toSlotIndex(parent);

        if ( !isValidSlot(parentSlotIndex) )
        {
            return QModelIndex();
        }

        const SlotVector& children = childrenOfParent(parentSlotIndex);
        if ( row < 0 || row >= children.count() )
        {
            return QModelIndex();
        }

        return createIndexForSlot(children.at(row), row, column);
    }

    QModelIndex ProfilerItemModelAdatper::parent(const QModelIndex &child) const
    {
        const int childSlotIndex = toSlotIndex(child);

        if ( !isValidSlot(childSlotIndex) )
        {
            return QModelIndex();
        }

        const int parentSlotIndex = m_Model.parent(childSlotIndex);
        if ( parentSlotIndex < 0 )
        {
            return QModelIndex();
        }

        const int grandparentSlotIndex = m_Model.parent(parentSlotIndex);
        const int parentIndexInGrandparentsChildren = childrenOfParent(grandparentSlotIndex).indexOf(parentSlotIndex);
        Q_ASSERT_X(parentIndexInGrandparentsChildren >= 0, Q_FUNC_INFO, "Expected parent to be present in grandparent's list of children!");

        return createIndex(parentIndexInGrandparentsChildren, 0, parentSlotIndex);
    }

    int ProfilerItemModelAdatper::rowCount(const QModelIndex &parent) const
    {
        const int parentSlotIndex = toSlotIndex(parent);
        if ( !isValidSlot(parentSlotIndex) )
        {
            return 0;
        }

        return childrenOfParent(parentSlotIndex).count();
    }

    int ProfilerItemModelAdatper::columnCount(const QModelIndex &parent) const
    {
        const int parentSlotIndex = toSlotIndex(parent);
        if ( !isValidSlot(parentSlotIndex) )
        {
            return 0;
        }

        return TOTAL_COLUMNS;
    }

    QVariant ProfilerItemModelAdatper::data(const QModelIndex &index, int role) const
    {
        const int slotIndex = toSlotIndex(index);
        if ( !isValidSlot(slotIndex) )
        {
            return QVariant();
        }

        // TODO: Replace placeholder data.
        if ( role != Qt::DisplayRole )
        {
            return QVariant();
        }

        return QVariant("Placeholder");
    }

    bool ProfilerItemModelAdatper::isValidSlot(int slotIndex) const
    {
        return slotIndex >= -1 && slotIndex < static_cast<int>(m_Model.dataSlotCount());
    }

    const ProfilerItemModelAdatper::SlotVector& ProfilerItemModelAdatper::childrenOfParent(int parentIndex) const
    {
        ++parentIndex;

        Q_ASSERT_X(parentIndex >= 0 && parentIndex < m_ChildLists.count(), Q_FUNC_INFO, "Invalid parent index!");

        return m_ChildLists[parentIndex];
    }

    QModelIndex ProfilerItemModelAdatper::createIndexForSlot(int slotIndexInProfilerModel, int slotIndexInParentsChildrenList, int column) const
    {
        return createIndex(slotIndexInParentsChildrenList, column, slotIndexInProfilerModel);
    }
}
