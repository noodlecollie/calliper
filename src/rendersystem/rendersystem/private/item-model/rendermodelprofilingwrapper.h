#ifndef RENDERMODELPROFILINGWRAPPER_H
#define RENDERMODELPROFILINGWRAPPER_H

#include <QAbstractItemModel>
#include <QVector>
#include <QHash>

#include "containers/adapters/iobjectstoreitemmodel.h"

#include "rendersystem/interface-classes/definitions/rendermodeldefs.h"

class RenderModelProfilingWrapper : public QAbstractItemModel
{
    Q_OBJECT
public:
    RenderModelProfilingWrapper(QObject* parent = Q_NULLPTR);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &index) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private slots:
    void renderModelStoreChanged();
    void profilerModelReset();

private:
    struct IndexRecord
    {
        RenderSystem::RenderModelDefs::RenderModelId m_nRenderModelId;
        QModelIndex m_InnerIndex;
        QModelIndex m_OuterParentIndex;

        IndexRecord(RenderSystem::RenderModelDefs::RenderModelId modelId, const QModelIndex& innerIndex, const QModelIndex& outerParentIndex)
            : m_nRenderModelId(modelId),
              m_InnerIndex(innerIndex),
              m_OuterParentIndex(outerParentIndex)
        {
        }

        IndexRecord()
            : m_nRenderModelId(RenderSystem::RenderModelDefs::INVALID_RENDER_MODEL_ID),
              m_InnerIndex(),
              m_OuterParentIndex()
        {
        }
    };

    struct ProfilerModelInfo
    {
        QModelIndex m_RootIndex;
        int m_nRowCount;

        ProfilerModelInfo(const QModelIndex& rootIndex, int rowCount)
            : m_RootIndex(rootIndex),
              m_nRowCount(rowCount)
        {
        }

        ProfilerModelInfo()
            : m_RootIndex(),
              m_nRowCount(0)
        {
        }

        inline bool isValid() const
        {
            return m_RootIndex.isValid();
        }
    };

    static RenderSystem::RenderModelDefs::RenderModelId renderModelIdFromRow(int row);
    static int renderModelCount();
    static QModelIndex innerModelIndex(int row, int column, const IndexRecord& parentIndexRecord);
    static int profilerModelRowCount(const IndexRecord& indexRecord);

    QModelIndex createNewIndexForOuterModel(int row, int column, const QModelIndex &parent) const;
    QModelIndex createNewIndexForInnerModel(int row, int column, const QModelIndex &parent) const;
    int getRowCountFromInnerModel(const QModelIndex& parent) const;
    void removeAllProfilerModelSignalMappings() const;
    void subscribeToResetNotifications(int row, const QModelIndex& rootIndex) const;

    mutable QVector<IndexRecord> m_IndexRecords;
    mutable QHash<QAbstractItemModel*, ProfilerModelInfo> m_ProfilerModelToModelId;
};

#endif // RENDERMODELPROFILINGWRAPPER_H
