#include "basescene.h"
#include "sceneobject.h"
#include "scenecamera.h"
#include "mapscene.h"
#include "uiscene.h"
#include "mapdocument.h"

namespace NS_RENDERER
{
    BaseScene::BaseScene(MapDocument *doc) : QObject(doc)
    {
        m_pRootObject = NULL;
        createRoot();

        // The internal objects node groups together things that aren't really
        // part of the scene, eg. the grid/origin marker.
        m_pInternalObjects = createSceneObject<SceneObject>(m_pRootObject);

        // The general objects node is for everything else.
        m_pGeneralObjects = createSceneObject<SceneObject>(m_pRootObject);
    }

    BaseScene::~BaseScene()
    {
        destroyRoot();
    }

    void BaseScene::createRoot()
    {
        Q_ASSERT(!m_pRootObject);
        m_pRootObject = new SceneObject(this, NULL);
        m_pRootObject->setObjectName("root");
        m_pRootObject->setUseCachedBounds(true);

    #ifdef LOG_SCENE_OBJECT_LIFETIMES
        qDebug() << "Root scene object" << m_pRootObject << "created";
    #endif

        emit sceneObjectCreated(m_pRootObject);
    }

    void BaseScene::destroyRoot()
    {
        Q_ASSERT(m_pRootObject);

    #ifdef LOG_SCENE_OBJECT_LIFETIMES
        qDebug() << "Scene object" << m_pRootObject << "about to be destroyed.";
    #endif

        emit subtreeDestroyed(m_pRootObject);
        delete m_pRootObject;
        m_pRootObject = NULL;
    }

    void BaseScene::clear()
    {
        destroyRoot();
        createRoot();
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

    void BaseScene::destroySceneObject(SceneObject *obj)
    {
        Q_ASSERT(obj != m_pRootObject);
        Q_ASSERT(obj->m_pScene == this);

    #ifdef LOG_SCENE_OBJECT_LIFETIMES
        qDebug() << "Scene object" << obj->objectName() << "about to be destroyed.";
    #endif

        emit subtreeDestroyed(obj);
        delete obj;
    }

    void BaseScene::processAddObject(SceneObject *obj)
    {
        Q_UNUSED(obj);
    }

    void BaseScene::processRemoveObject(SceneObject *obj)
    {
        Q_UNUSED(obj);
    }

    SceneObject* BaseScene::generalObjectsNode() const
    {
        return m_pGeneralObjects;
    }
}