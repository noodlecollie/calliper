#ifndef RENDERMODELSTORE_H
#define RENDERMODELSTORE_H

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
    virtual RenderModelId createRenderModel() override;
    virtual void removeRenderModel(RenderModelId modelId) override;
    virtual void addGeometry(RenderModelId modelId, const RenderSystem::GeometryBuilder& builder) override;
    virtual void removeGeometry(RenderModelId modelId, ObjectId objectId) override;
    virtual void clearGeometry(RenderModelId modelId) override;

    virtual void draw(const RenderModelId modelId,
                      const FrameBufferId frameBufferId,
                      const RenderMode renderMode,
                      const QMatrix4x4 &worldToCameraMatrix,
                      const QMatrix4x4 &projectionMatrix) override;
};

#endif // RENDERMODELSTORE_H
