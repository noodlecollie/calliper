#ifndef RENDERMODELPROFILINGWRAPPER_H
#define RENDERMODELPROFILINGWRAPPER_H

#include <QAbstractItemModel>
#include <QVector>

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

    mutable QVector<IndexRecord> m_IndexRecords;
};

#endif // RENDERMODELPROFILINGWRAPPER_H
