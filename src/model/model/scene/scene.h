#ifndef SCENE_H
#define SCENE_H

#include "model_global.h"
#include "sceneobjectinitparams.h"

#include "model/scene/sceneobject.h"

#include "rendersystem/interface-classes/definitions/publicrendermodeldefs.h"

namespace Model
{
    class MODELSHARED_EXPORT Scene : public QObject
    {
        Q_OBJECT
    public:
        explicit Scene(QObject* parent = 0);

        template<typename T, typename... Args>
        T* createSceneObject(Args... args)
        {
            T* obj = new T(SceneObjectInitParams(this, acquireNextObjectId()), std::move(args)...);
            processSceneObjectCreated(obj);
            return obj;
        }

        template<typename T>
        T* cloneSceneObject(const T* cloneFrom)
        {
            Q_ASSERT_X(cloneFrom, Q_FUNC_INFO, "Object to clone from is null!");
            Q_ASSERT_X(cloneFrom->scene() == this, Q_FUNC_INFO, "Cannot clone object from a different scene!");
            T* obj = new T(SceneObjectCloneParams(this, acquireNextObjectId(), cloneFrom));
            processSceneObjectCloned(obj);
            return obj;
        }

        void destroySceneObject(SceneObject* object);
        void clear();
        int sceneObjectCount() const;

        SceneObject* rootObject() const;

    signals:
        void objectCreated(SceneObject*);
        void objectDestroyed(SceneObject*);

    private:
        void processSceneObjectCreated(SceneObject* object);
        void processSceneObjectCloned(SceneObject* object);
        RenderSystem::PublicRenderModelDefs::ObjectId acquireNextObjectId();
        void addObjectToTable(SceneObject* object);
        void removeObjectFromTable(SceneObject* object);
        void deleteObjectsRecursive(SceneObject* object);

        RenderSystem::PublicRenderModelDefs::ObjectId m_iObjectIdCounter;
        SceneObject* m_pRootObject;
        QHash<RenderSystem::PublicRenderModelDefs::ObjectId, SceneObject*> m_ObjectTable;
    };
}

#endif // SCENE_H
