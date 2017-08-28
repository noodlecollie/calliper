#ifndef RENDERMODELSTORE_H
#define RENDERMODELSTORE_H

#include "rendermodelstoreitemmodeladapter.h"

#include "rendersystem/interfaces/irendermodelstore.h"
#include "rendersystem/private/rendermodel/rendermodel.h"
#include "rendersystem/interface-classes/definitions/rendermodeldefs.h"

#include "containers/itempointer/itempointerbasedobjectstore.h"

#include "calliperutil/global/globalinstancehelper.h"

class RenderModelStore : public Containers::ItemPointerBasedObjectStore<RenderModel,
                                                                        RenderSystem::RenderModelDefs::RenderModelId>,
                         public RenderSystem::IRenderModelStore,
                         public CalliperUtil::GlobalInstanceHelper<RenderModelStore>
{
public:
    typedef RenderSystem::RenderModelDefs::RenderModelId RenderModelId;
    typedef RenderSystem::RenderModelDefs::ObjectId ObjectId;
    typedef RenderSystem::FrameBufferDefs::FrameBufferId FrameBufferId;
    typedef RenderSystem::ShaderDefs::RenderMode RenderMode;

    RenderModelStore();
    virtual ~RenderModelStore();

    // External
    virtual RenderModelId createRenderModel(const QString& name) override;
    virtual QString name(const RenderModelId modelId) const override;
    virtual void setName(const RenderModelId modelId, const QString &newName) const override;
    virtual void removeRenderModel(const RenderModelId modelId) override;
    virtual void addGeometry(const RenderModelId modelId, const RenderSystem::GeometryBuilder& builder) override;
    virtual void removeGeometry(const RenderModelId modelId, const ObjectId objectId) override;
    virtual void clearGeometry(const RenderModelId modelId) override;
    virtual void draw(const RenderModelId modelId,
                      const FrameBufferId frameBufferId,
                      const RenderSystem::FrameDrawParams& drawParams) override;
    virtual QAbstractItemModel* profilingData(const RenderModelId modelId) const override;
    virtual QAbstractItemModel* itemModel() const override;

    // Internal
    Containers::IObjectStoreItemModel* objectStoreItemModel() const;

private:
    mutable RenderModelStoreItemModelAdapter m_ItemModelAdapter;
};

#endif // RENDERMODELSTORE_H
