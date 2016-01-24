#include "uiscene.h"
#include "scenecamera.h"

UIScene::UIScene() : BaseScene()
{
    m_pCamera = new SceneCamera(m_pRootObject);

    // TODO: Set up camera.
}

UIScene::~UIScene()
{

}

BaseScene::SceneType UIScene::sceneType() const
{
    return TypeUI;
}

SceneCamera* UIScene::camera() const
{
    return m_pCamera;
}

bool UIScene::hasUIElements() const
{
    QList<SceneObject*> list = m_pRootObject->children();
    list.removeOne(m_pCamera);

    return list.count() > 0;
}
