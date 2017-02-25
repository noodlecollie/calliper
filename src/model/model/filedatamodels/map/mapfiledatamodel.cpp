#include "mapfiledatamodel.h"

namespace Model
{
    MapFileDataModel::MapFileDataModel()
        : BaseFileDataModel(),
          m_pScene(new MapScene())
    {

    }

    MapFileDataModel::~MapFileDataModel()
    {

    }

    BaseFileDataModel::FileType MapFileDataModel::type() const
    {
        return MapFile;
    }

    MapScene* MapFileDataModel::scene()
    {
        return m_pScene.data();
    }

    const MapScene* MapFileDataModel::scene() const
    {
        return m_pScene.data();
    }
}
