#include "mapscene.h"
#include "scenecamera.h"
#include "sceneobject.h"
#include "basegrid.h"

MapScene::MapScene(MapDocument *document)
{
    m_pDocument = document;
    m_pRootObject = new SceneObject(NULL);
    m_pRootObject->m_pScene = this;
    m_pRootObject->setObjectName("root");
    m_pGrid = NULL;
}

SceneObject* MapScene::root() const
{
    return m_pRootObject;
}

MapScene::~MapScene()
{
    delete m_pRootObject;
}

QList<SceneCamera*> MapScene::findCameras() const
{
    return m_pRootObject->findChildren<SceneCamera*>(QString(), Qt::FindChildrenRecursively);
}

QList<SceneObject*> MapScene::findByName(const QString &name)
{
    return m_pRootObject->findChildren<SceneObject*>(name, Qt::FindChildrenRecursively);
}

MapDocument* MapScene::document() const
{
    return m_pDocument;
}

BaseGrid* MapScene::grid() const
{
    return m_pGrid;
}
