#include "basescene.h"
#include "sceneobject.h"
#include "scenecamera.h"

BaseScene::BaseScene()
{
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

BaseScene::SceneType BaseScene::sceneType() const
{
    return TypeBase;
}
