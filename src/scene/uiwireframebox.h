#ifndef UIWIREFRAMEBOX_H
#define UIWIREFRAMEBOX_H

#include "uimanipulator.h"

class UIWireframeBox : public UIManipulator
{
    Q_OBJECT
    friend class BaseScene;
public:

protected:
    explicit UIWireframeBox(BaseScene* scene, SceneObject *parent);
    UIWireframeBox(const UIWireframeBox &cloneFrom);
    virtual ~UIWireframeBox();
};

#endif // UIWIREFRAMEBOX_H
