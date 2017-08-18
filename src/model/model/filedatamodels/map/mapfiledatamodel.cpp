#include "mapfiledatamodel.h"

#include "rendersystem/endpoints/rendermodelstoreendpoint.h"

namespace Model
{
    MapFileDataModel::MapFileDataModel()
        : BaseFileDataModel(),
          m_pScene(new MapScene()),
          m_nRenderModelId(RenderSystem::RenderModelDefs::INVALID_RENDER_MODEL_ID)
    {
        using namespace RenderSystem;

        RenderModelStoreEndpoint::RenderModelStoreAccessor renderModelStore = RenderModelStoreEndpoint::renderModelStore();
        m_nRenderModelId = renderModelStore->createRenderModel(QString());

        Q_ASSERT_X(m_nRenderModelId != RenderModelDefs::INVALID_RENDER_MODEL_ID,
                   Q_FUNC_INFO,
                   "Expected valid render model ID!");
    }

    MapFileDataModel::~MapFileDataModel()
    {
        using namespace RenderSystem;

        if ( m_nRenderModelId != RenderModelDefs::INVALID_RENDER_MODEL_ID )
        {
            RenderModelStoreEndpoint::RenderModelStoreAccessor renderModelStore = RenderModelStoreEndpoint::renderModelStore();
            renderModelStore->removeRenderModel(m_nRenderModelId);
            m_nRenderModelId = RenderModelDefs::INVALID_RENDER_MODEL_ID;
        }
    }

    BaseFileDataModel::ModelType MapFileDataModel::type() const
    {
        return MapModel;
    }

    MapScene* MapFileDataModel::scene()
    {
        return m_pScene.data();
    }

    const MapScene* MapFileDataModel::scene() const
    {
        return m_pScene.data();
    }

    RenderSystem::RenderModelDefs::RenderModelId MapFileDataModel::renderModelId() const
    {
        return m_nRenderModelId;
    }

    void MapFileDataModel::setRenderModelName(const QString &name) const
    {
        RenderSystem::RenderModelStoreEndpoint::constRenderModelStore()->setName(m_nRenderModelId, name);
    }
}
