#ifndef SCENE_H
#define SCENE_H

#include "sceneobject.h"

class Scene
{
public:
    Scene();

    SceneObject* root() const;

private:
    SceneObject*    m_pRootObject;
};

#endif // SCENE_H
