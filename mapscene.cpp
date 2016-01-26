#include "mapscene.h"
#include "scenecamera.h"
#include "sceneobject.h"
#include "mapgrid.h"

MapScene::MapScene(MapDocument *document) : BaseScene(document)
{
    m_pGrid = NULL;
}

MapScene::~MapScene()
{
}

BaseScene::SceneType MapScene::type() const
{
    return TypeMapScene;
}

MapGrid* MapScene::grid() const
{
    return m_pGrid;
}
