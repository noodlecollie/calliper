#include "uiwireframebox.h"
#include "basescene.h"

UIWireframeBox::UIWireframeBox(BaseScene *scene, SceneObject *parent) : UIManipulator(scene, parent)
{

}

UIWireframeBox::~UIWireframeBox()
{

}

SceneObject* UIWireframeBox::clone() const
{
    return m_pScene->cloneSceneObject<UIWireframeBox>(this);
}

UIWireframeBox::UIWireframeBox(const UIWireframeBox &cloneFrom) : UIManipulator(cloneFrom)
{

}
