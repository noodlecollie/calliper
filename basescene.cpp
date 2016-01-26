#include "basescene.h"
#include "sceneobject.h"
#include "scenecamera.h"
#include "mapscene.h"
#include "uiscene.h"

BaseScene::BaseScene(MapDocument *doc)
{
    m_pDocument = doc;
    m_pRootObject = new SceneObject(NULL);
    m_pRootObject->m_pScene = this;
    m_pRootObject->setObjectName("root");
}

BaseScene::~BaseScene()
{
    delete m_pRootObject;
}

SceneObject* BaseScene::root() const
{
    return m_pRootObject;
}

QList<SceneCamera*> BaseScene::findCameras() const
{
    return m_pRootObject->findChildren<SceneCamera*>(QString(), Qt::FindChildrenRecursively);
}

QList<SceneObject*> BaseScene::findByName(const QString &name)
{
    return m_pRootObject->findChildren<SceneObject*>(name, Qt::FindChildrenRecursively);
}

BaseScene::SceneType BaseScene::type() const
{
    return TypeBaseScene;
}

MapScene* BaseScene::mapScene()
{
    return dynamic_cast<MapScene*>(this);
}

const MapScene* BaseScene::mapScene() const
{
    return dynamic_cast<const MapScene*>(this);
}

UIScene* BaseScene::uiScene()
{
    return dynamic_cast<UIScene*>(this);
}

const UIScene* BaseScene::uiScene() const
{
    return dynamic_cast<const UIScene*>(this);
}

MapDocument* BaseScene::document() const
{
    return m_pDocument;
}
