#include "scene.h"

#include "rendersystem/endpoints/rendermodelstoreendpoint.h"

namespace Model
{
    Scene::Scene(QObject* parent)
        : QObject(parent),
          m_iObjectIdCounter(0),
          m_pRootObject(new SceneObject(SceneObjectInitParams(this, acquireNextObjectId()), Q_NULLPTR))
    {
        addObjectToTable(m_pRootObject);
        m_pRootObject->setMustExist(true);
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

        if ( object->mustExist() )
        {
            object->setParentObject(m_pRootObject);
            return;
        }

        emit objectDestroyed(object);
        removeObjectFromTable(object);
        delete object;
    }

    void Scene::clear()
    {
        QList<SceneObject*> children = m_pRootObject->childSceneObjects();
        foreach ( SceneObject* object, children )
        {
            destroySceneObject(object);
        }
    }

    int Scene::sceneObjectCount() const
    {
        return m_ObjectTable.count();
    }

    SceneObject* Scene::rootObject() const
    {
        return m_pRootObject;
    }

    RenderSystem::RenderModelDefs::ObjectId Scene::acquireNextObjectId()
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

    // TODO: This should be event-driven! Iterating over everything each frame is slow.
    void Scene::updateRenderGeometry(RenderSystem::RenderModelDefs::RenderModelId renderModelId)
    {
        using namespace RenderSystem;

        typedef QHash<RenderSystem::RenderModelDefs::ObjectId, SceneObject*> ObjectTable;

        RenderModelStoreEndpoint::RenderModelStoreAccessor renderModelStore = RenderModelStoreEndpoint::renderModelStore();

        for ( ObjectTable::const_iterator it = m_ObjectTable.begin(); it != m_ObjectTable.end(); ++it )
        {
            SceneObject* object = it.value();
            if ( !object->needsRendererUpdate() )
            {
                continue;
            }

            GeometryBuilder builder(object->objectId(), MaterialDefs::INVALID_MATERIAL_ID, object->localToRootMatrix());
            object->rendererUpdate(builder);
            renderModelStore->addGeometry(renderModelId, builder);
        }
    }
}
