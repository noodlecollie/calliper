#ifndef RENDERMODELSTOREITEMMODELADAPTER_H
#define RENDERMODELSTOREITEMMODELADAPTER_H

#include "rendersystem/private/rendermodel/rendermodel.h"
#include "rendersystem/interface-classes/definitions/rendermodeldefs.h"

#include "containers/adapters/itempointerbaseditemmodeladapter.h"

class RenderModelStore;

class RenderModelStoreItemModelAdapter : public Containers::ItemPointerBasedItemModelAdapter<RenderModel,
                                                                                             RenderSystem::RenderModelDefs::RenderModelId>
{
public:
    enum ModelColumn
    {
        PlaceholderColumn = 0,

        TOTAL_MODEL_COLUMNS
    };

    RenderModelStoreItemModelAdapter(const RenderModelStore& store);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual int columnCount(const QModelIndex &parent) const override;

protected:
    virtual QVariant itemData(const ObjectId &id, const QModelIndex &index, int role) const override;

private:
    const RenderModelStore& m_Store;
};

#endif // RENDERMODELSTOREITEMMODELADAPTER_H
