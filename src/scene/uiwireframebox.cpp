#include "uiwireframebox.h"
#include "basescene.h"

UIWireframeBox::UIWireframeBox(BaseScene *scene, SceneObject *parent) : UIManipulator(scene, parent)
{

}

UIWireframeBox::~UIWireframeBox()
{

}

UIWireframeBox::UIWireframeBox(const UIWireframeBox &cloneFrom) : UIManipulator(cloneFrom)
{

}
