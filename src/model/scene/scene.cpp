#include "scene.h"

namespace Model
{
    Scene::Scene(ShaderStore* shaderStore, TextureStore* textureStore, MaterialStore* materialStore, QObject* parent)
        : QObject(parent),
          m_pShaderStore(shaderStore),
          m_pTextureStore(textureStore),
          m_pMaterialStore(materialStore),
          m_iObjectIdCounter(0),
          m_pRootObject(new SceneObject(SceneObjectInitParams(this, acquireNextObjectId()), nullptr))
    {
        Q_ASSERT_X(m_pShaderStore, Q_FUNC_INFO, "Shader store cannot be null!");
        Q_ASSERT_X(m_pTextureStore, Q_FUNC_INFO, "Texture store cannot be null!");
        addObjectToTable(m_pRootObject);
    }

    void Scene::processSceneObjectCreated(SceneObject *object)
    {
        Q_ASSERT_X(object->parentObject(), Q_FUNC_INFO, "Scene object must have a parent object!");
        Q_ASSERT_X(object->parentScene() == this, Q_FUNC_INFO, "Scene object must belong to this scene!");
        Q_ASSERT_X(object->objectId() > 0, Q_FUNC_INFO, "Object must have a valid ID!");

        addObjectToTable(object);

        emit objectCreated(object);
    }

    void Scene::processSceneObjectCloned(SceneObject *object)
    {
        Q_ASSERT_X(object->parentObject(), Q_FUNC_INFO, "Scene object must have a parent object!");
        Q_ASSERT_X(object->parentScene() == this, Q_FUNC_INFO, "Scene object must belong to this scene!");
        Q_ASSERT_X(object->objectId() > 0, Q_FUNC_INFO, "Object must have a valid ID!");

        addObjectToTable(object);

        emit objectCreated(object);
    }

    void Scene::destroySceneObject(SceneObject *object)
    {
        Q_ASSERT_X(object->parentScene() == this, Q_FUNC_INFO, "Scene object must belong to this scene!");
        Q_ASSERT_X(object != m_pRootObject, Q_FUNC_INFO, "Cannot delete the root object!");
        if ( object->parentScene() != this || object == m_pRootObject )
            return;

        deleteObjectsRecursive(object);
    }

    void Scene::deleteObjectsRecursive(SceneObject *object)
    {
        QList<SceneObject*> children = object->childSceneObjects();
        foreach ( SceneObject* object, children )
        {
            deleteObjectsRecursive(object);
        }

        emit objectDestroyed(object);
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

    ShaderStore* Scene::shaderStore() const
    {
        return m_pShaderStore;
    }

    TextureStore* Scene::textureStore() const
    {
        return m_pTextureStore;
    }
}
