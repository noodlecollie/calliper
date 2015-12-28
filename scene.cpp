#include "scene.h"
#include "camera.h"
#include "sceneobject.h"

Scene::Scene()
{
    m_pRootObject = new SceneObject(NULL);
    m_pRootObject->m_pScene = this;
}

SceneObject* Scene::root() const
{
    return m_pRootObject;
}

Scene::~Scene()
{
    delete m_pRootObject;
}

QList<Camera*> Scene::findCameras() const
{
    return m_pRootObject->findChildren<Camera*>(QString(), Qt::FindChildrenRecursively);
}

QList<SceneObject*> Scene::findByName(const QString &name)
{
    return m_pRootObject->findChildren<SceneObject*>(name, Qt::FindChildrenRecursively);
}
