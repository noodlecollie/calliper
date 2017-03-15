#include "mapfiledatamodel.h"
#include "model/global/resourceenvironment.h"

namespace Model
{
    MapFileDataModel::MapFileDataModel()
        : BaseFileDataModel(),
          m_pScene(new MapScene())
    {
        m_RenderModel.setRenderFunctors(ResourceEnvironment::globalInstance()->renderFunctors());
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

    Renderer::RenderModel* MapFileDataModel::renderModel()
    {
        return &m_RenderModel;
    }

    const Renderer::RenderModel* MapFileDataModel::renderModel() const
    {
        return &m_RenderModel;
    }
}
