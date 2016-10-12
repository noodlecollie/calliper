#include "scene.h"

namespace NS_MODEL
{
    Scene::Scene(QObject* parent)
        : QObject(parent), m_pRootObject(new SceneObject(this, nullptr)),
          m_iObjectIdCounter(0)
    {

    }

    void Scene::processSceneObjectCreated(SceneObject *object)
    {
        object->m_iObjectId = acquireNextObjectId();
        Q_ASSERT_X(object->parentObject(), Q_FUNC_INFO, "Scene object must have a parent object!");
        Q_ASSERT_X(object->parentScene() == this, Q_FUNC_INFO, "Scene object must belong to this scene!");
        Q_ASSERT_X(object->objectId() > 0, Q_FUNC_INFO, "Object must have a valid ID!");

        // TODO: Emit signals
    }

    void Scene::processSceneObjectCloned(SceneObject *object)
    {
        object->m_iObjectId = acquireNextObjectId();
        Q_ASSERT_X(object->parentObject(), Q_FUNC_INFO, "Scene object must have a parent object!");
        Q_ASSERT_X(object->parentScene() == this, Q_FUNC_INFO, "Scene object must belong to this scene!");
        Q_ASSERT_X(object->objectId() > 0, Q_FUNC_INFO, "Object must have a valid ID!");

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

    void Scene::acquireNextObjectId()
    {
        Q_ASSERT_X(m_iObjectIdCounter + 1 > 0, Q_FUNC_INFO, "How on earth did you manage to overflow this??");
        return m_iObjectIdCounter++;
    }
}
