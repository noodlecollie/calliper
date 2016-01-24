#include "mapscene.h"
#include "scenecamera.h"
#include "sceneobject.h"
#include "basegrid.h"

MapScene::MapScene(MapDocument *document) : BaseScene()
{
    m_pDocument = document;
    m_pGrid = NULL;
}

MapScene::~MapScene()
{

}

MapDocument* MapScene::document() const
{
    return m_pDocument;
}

BaseGrid* MapScene::grid() const
{
    return m_pGrid;
}

BaseScene::SceneType MapScene::sceneType() const
{
    return TypeMap;
}
