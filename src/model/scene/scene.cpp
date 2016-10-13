#include "scene.h"

namespace NS_MODEL
{
    Scene::Scene(QObject* parent)
        : QObject(parent),
          m_iObjectIdCounter(0),
          m_pRootObject(new SceneObject(SceneObjectInitParams(this, acquireNextObjectId()), nullptr))
    {
        addObjectToTable(m_pRootObject);
    }

    void Scene::processSceneObjectCreated(SceneObject *object)
    {
        Q_ASSERT_X(object->parentObject(), Q_FUNC_INFO, "Scene object must have a parent object!");
        Q_ASSERT_X(object->parentScene() == this, Q_FUNC_INFO, "Scene object must belong to this scene!");
        Q_ASSERT_X(object->objectId() > 0, Q_FUNC_INFO, "Object must have a valid ID!");

        addObjectToTable(object);

        // TODO: Emit signals
    }

    void Scene::processSceneObjectCloned(SceneObject *object)
    {
        Q_ASSERT_X(object->parentObject(), Q_FUNC_INFO, "Scene object must have a parent object!");
        Q_ASSERT_X(object->parentScene() == this, Q_FUNC_INFO, "Scene object must belong to this scene!");
        Q_ASSERT_X(object->objectId() > 0, Q_FUNC_INFO, "Object must have a valid ID!");

        addObjectToTable(object);

        // TODO: Emit signals
    }

    void Scene::destroySceneObject(SceneObject *object)
    {
        Q_ASSERT_X(object->parentScene() == this, Q_FUNC_INFO, "Scene object must belong to this scene!");
        Q_ASSERT_X(object != m_pRootObject, Q_FUNC_INFO, "Cannot delete the root object!");

        // TODO: Make recursive and emit signals
        removeObjectFromTable(object);
        delete object;
    }

    SceneObject* Scene::rootObject() const
    {
        return m_pRootObject;
    }

    quint32 Scene::acquireNextObjectId()
    {
        Q_ASSERT_X(m_iObjectIdCounter + 1 > 0, Q_FUNC_INFO, "How on earth did you manage to overflow this??");
        return ++m_iObjectIdCounter;
    }

    void Scene::addObjectToTable(SceneObject *object)
    {
        Q_ASSERT_X(object->objectId() > 0, Q_FUNC_INFO, "Object cannot have an ID of zero!");
        Q_ASSERT_X(!m_ObjectTable.contains(object->objectId()), Q_FUNC_INFO, "Object already exists!");
        m_ObjectTable.insert(object->objectId(), object);
    }

    void Scene::removeObjectFromTable(SceneObject *object)
    {
        Q_ASSERT_X(object->objectId() > 0, Q_FUNC_INFO, "Object cannot have an ID of zero!");
        m_ObjectTable.remove(object->objectId());
    }
}
