#ifndef UISCENE_H
#define UISCENE_H

#include "basescene.h"

class UIScene : public BaseScene
{
public:
    UIScene();
    virtual ~UIScene();
    virtual SceneType sceneType() const;
};

#endif // UISCENE_H
