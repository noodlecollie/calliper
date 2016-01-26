#include "uiscene.h"

UIScene::UIScene(MapDocument *document) : BaseScene(document)
{
    m_pDocument = document;
}

UIScene::~UIScene()
{

}

BaseScene::SceneType UIScene::type() const
{
    return TypeUIScene;
}
