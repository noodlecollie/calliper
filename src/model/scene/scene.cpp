#include "scene.h"

namespace NS_MODEL
{
    Scene::Scene(QObject* parent)
        : QObject(parent)
    {

    }

    void Scene::processSceneObjectCreated(SceneObject *object)
    {
        // TODO: Emit signals
    }

    void Scene::processSceneObjectCloned(SceneObject *object)
    {
        // TODO: Emit signals
    }

    void Scene::destroySceneObject(SceneObject *obj)
    {
        // TODO: Make recursive and emit signals
        delete obj;
    }
}
