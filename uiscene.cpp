#include "uiscene.h"

UIScene::UIScene() : BaseScene()
{

}

UIScene::~UIScene()
{

}

BaseScene::SceneType UIScene::sceneType() const
{
    return TypeUI;
}

