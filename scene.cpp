#include "scene.h"
#include "camera.h"
#include "sceneobject.h"
#include "basegrid.h"

Scene::Scene(MapDocument *document)
{
    m_pDocument = document;
    m_pRootObject = new SceneObject(NULL);
    m_pRootObject->m_pScene = this;
    m_pRootObject->setObjectName("root");
    m_pGrid = NULL;
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
    Q_ASSERT(!qobject_cast<Camera*>(m_pRootObject));
    return m_pRootObject->findChildren<Camera*>(QString(), Qt::FindChildrenRecursively);
}

QList<SceneObject*> Scene::findByName(const QString &name)
{
    return m_pRootObject->findChildren<SceneObject*>(name, Qt::FindChildrenRecursively);
}

MapDocument* Scene::document() const
{
    return m_pDocument;
}

BaseGrid* Scene::grid() const
{
    return m_pGrid;
}
