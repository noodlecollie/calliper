#include "basescene.h"
#include "sceneobject.h"
#include "scenecamera.h"
#include "mapscene.h"
#include "uiscene.h"
#include "mapdocument.h"

BaseScene::BaseScene(MapDocument *doc) : QObject(doc)
{
    createNewRoot();
}

BaseScene::~BaseScene()
{
}

void BaseScene::clear()
{
    destroySceneObject(m_pRootObject);
    sceneClearedEvent();
    createNewRoot();
}

void BaseScene::createNewRoot()
{
    // Ensure this is set to NULL before we attempt to create a root,
    // because the root object itself does debug verification that depends on this.
    m_pRootObject = NULL;

    m_pRootObject = new SceneObject(this, NULL);
    m_pRootObject->setParent(this);
    m_pRootObject->setObjectName("root");
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
    return qobject_cast<MapDocument*>(parent());
}

SceneObject* BaseScene::unserialiseSceneObject(const QJsonObject &serialisedData, SceneObject *parent)
{
    // TODO
    return NULL;
}

void BaseScene::destroySceneObject(SceneObject *obj)
{
    Q_ASSERT(obj != m_pRootObject);
    Q_ASSERT(obj->m_pScene == this);
    emit subtreeDestroyed(obj);
    delete obj;
}

void BaseScene::sceneClearedEvent()
{
}
