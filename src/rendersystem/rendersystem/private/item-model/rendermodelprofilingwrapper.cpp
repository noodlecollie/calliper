#include "rendermodelprofilingwrapper.h"

#include "rendersystem/private/stores/rendermodelstore/rendermodelstore.h"

RenderModelProfilingWrapper::RenderModelProfilingWrapper(QObject *parent)
    : QAbstractItemModel(parent)
{

}

QModelIndex RenderModelProfilingWrapper::index(int row, int column, const QModelIndex &parent) const
{
    using namespace RenderSystem;

    // Sanity check.
    // TODO: Make functional with more columns.
    if ( row < 0 || column != 0 )
    {
        return QModelIndex();
    }

    RenderModelStore* const renderModelStore = RenderModelStore::globalInstance();
    Containers::IObjectStoreItemModel* const objectStoreItemModel = renderModelStore->objectStoreItemModel();

    if ( !parent.isValid() )
    {
        // The item doesn't belong to any inner model.
        // It's a top-level item in the outer model, and corresponds to a render model.
        // The render model itself is determined by the row.

        // Sanity check again.
        if ( row >= objectStoreItemModel->rowCount()  )
        {
            return QModelIndex();
        }

        const QVariant modelIdVariant = objectStoreItemModel->objectIdForRow(row);
        Q_ASSERT_X(modelIdVariant.isValid(), Q_FUNC_INFO, "Expected valid render model ID variant!");

        const RenderModelDefs::RenderModelId modelId = modelIdVariant.value<RenderModelDefs::RenderModelId>();
        Q_ASSERT_X(modelId != RenderModelDefs::INVALID_RENDER_MODEL_ID, Q_FUNC_INFO, "Expected valid render model ID!");

        m_IndexRecords.append(IndexRecord(modelId, QModelIndex(), parent));
        return createIndex(row, column, static_cast<quintptr>(m_IndexRecords.count() - 1));
    }

    // The item belongs to an inner model.
    // Get more information from the parent index.

    const int parentIndexRecordId = static_cast<int>(parent.internalId());
    if ( parentIndexRecordId < 0 || parentIndexRecordId >= m_IndexRecords.count() )
    {
        Q_ASSERT_X(false, Q_FUNC_INFO, "Expected valid index record ID for parent!");
        return QModelIndex();
    }

    const IndexRecord& parentIndexRecord = m_IndexRecords.at(parentIndexRecordId);

    // Generate an inner index from the inner model.
    Profiling::ProfilerItemModelAdatper* const profilerModel = renderModelStore->profilingData(parentIndexRecord.m_nRenderModelId);
    const QModelIndex innerIndex = profilerModel->index(row, column, parentIndexRecord.m_InnerIndex);

    m_IndexRecords.append(IndexRecord(parentIndexRecord.m_nRenderModelId, innerIndex, parent));
    return createIndex(row, column, static_cast<quintptr>(m_IndexRecords.count() - 1));
}

QModelIndex RenderModelProfilingWrapper::parent(const QModelIndex &index) const
{
    const int indexRecordId = static_cast<int>(index.internalId());
    if ( indexRecordId < 0 || indexRecordId >= m_IndexRecords.count() )
    {
        return QModelIndex();
    }

    return m_IndexRecords.at(indexRecordId).m_OuterParentIndex;
}

int RenderModelProfilingWrapper::rowCount(const QModelIndex &parent) const
{
    using namespace RenderSystem;

    RenderModelStore* const renderModelStore = RenderModelStore::globalInstance();

    if ( !parent.isValid() )
    {
        Containers::IObjectStoreItemModel* const objectStoreItemModel = renderModelStore->objectStoreItemModel();
        return objectStoreItemModel->rowCount();
    }

    const int indexRecordId = static_cast<int>(parent.internalId());
    if ( indexRecordId < 0 || indexRecordId >= m_IndexRecords.count() )
    {
        Q_ASSERT_X(false, Q_FUNC_INFO, "Expected valid index record ID!");
        return 0;
    }

    const IndexRecord& indexRecord = m_IndexRecords.at(indexRecordId);
    const RenderModelDefs::RenderModelId renderModelId = indexRecord.m_nRenderModelId;
    Profiling::ProfilerItemModelAdatper* const profilerModel = renderModelStore->profilingData(renderModelId);
    return profilerModel->rowCount(indexRecord.m_InnerIndex);
}

int RenderModelProfilingWrapper::columnCount(const QModelIndex &parent) const
{
    // TODO: Make functional with more columns.
    Q_UNUSED(parent);
    return 1;
}

QVariant RenderModelProfilingWrapper::data(const QModelIndex &index, int role) const
{
    // TODO: Make functional with more roles.
    if ( role != Qt::DisplayRole )
    {
        return QVariant();
    }

    const int indexRecordId = static_cast<int>(index.internalId());
    if ( indexRecordId < 0 || indexRecordId >= m_IndexRecords.count() )
    {
        Q_ASSERT_X(false, Q_FUNC_INFO, "Expected valid index record ID!");
        return 0;
    }

    const IndexRecord& indexRecord = m_IndexRecords.at(indexRecordId);
    if ( !indexRecord.m_InnerIndex.isValid() )
    {
        return QVariant(indexRecord.m_nRenderModelId);
    }

    RenderModelStore* const renderModelStore = RenderModelStore::globalInstance();
    Profiling::ProfilerItemModelAdatper* const profilerModel = renderModelStore->profilingData(indexRecord.m_nRenderModelId);
    return profilerModel->data(indexRecord.m_InnerIndex, role);
}
