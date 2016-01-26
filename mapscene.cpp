#include "mapscene.h"
#include "scenecamera.h"
#include "sceneobject.h"
#include "basegrid.h"

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

BaseGrid* MapScene::grid() const
{
    return m_pGrid;
}
