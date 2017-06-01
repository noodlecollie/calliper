#include "mapfiledatamodel.h"

namespace Model
{
    MapFileDataModel::MapFileDataModel()
        : BaseFileDataModel(),
          m_pScene(new MapScene()),
          m_nRenderModelId(RenderSystem::RenderModelDefs::INVALID_RENDER_MODEL_ID)
    {

    }

    MapFileDataModel::~MapFileDataModel()
    {

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

    void MapFileDataModel::setRenderModelId(RenderSystem::RenderModelDefs::RenderModelId renderModelId)
    {
        m_nRenderModelId = renderModelId;
    }
}
