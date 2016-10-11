#include "scene.h"

namespace NS_MODEL
{
    Scene::Scene(QObject* parent)
        : QObject(parent), m_pRootObject(new SceneObject(this, nullptr))
    {

    }

    void Scene::processSceneObjectCreated(SceneObject *object)
    {
        Q_ASSERT_X(object->parentObject(), Q_FUNC_INFO, "Scene object must have a parent object!");
        Q_ASSERT_X(object->parentScene() == this, Q_FUNC_INFO, "Scene object must belong to this scene!");

        // TODO: Emit signals
    }

    void Scene::processSceneObjectCloned(SceneObject *object)
    {
        Q_ASSERT_X(object->parentObject(), Q_FUNC_INFO, "Scene object must have a parent object!");
        Q_ASSERT_X(object->parentScene() == this, Q_FUNC_INFO, "Scene object must belong to this scene!");

        // TODO: Emit signals
    }

    void Scene::destroySceneObject(SceneObject *object)
    {
        Q_ASSERT_X(object->parentScene() == this, Q_FUNC_INFO, "Scene object must belong to this scene!");

        // TODO: Make recursive and emit signals
        delete object;
    }

    SceneObject* Scene::rootObject() const
    {
        return m_pRootObject;
    }
}
