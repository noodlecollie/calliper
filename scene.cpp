#include "scene.h"

Scene::Scene()
{
    m_pRootObject = new SceneObject(NULL);
}

SceneObject* Scene::root() const
{
    return m_pRootObject;
}
