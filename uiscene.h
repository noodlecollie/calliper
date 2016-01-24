#ifndef UISCENE_H
#define UISCENE_H

#include "basescene.h"

class SceneCamera;

class UIScene : public BaseScene
{
public:
    UIScene();
    virtual ~UIScene();
    virtual SceneType sceneType() const;

    SceneCamera* camera() const;
    bool hasUIElements() const;

private:
    SceneCamera*    m_pCamera;
};

#endif // UISCENE_H
