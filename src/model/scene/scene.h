#ifndef SCENE_H
#define SCENE_H

#include "model_global.h"
#include "scene/sceneobject.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT Scene : public QObject
    {
        Q_OBJECT
    public:
        explicit Scene(QObject* parent = 0);

        template<typename T, typename... Args>
        T* createSceneObject(Args... args)
        {
            T* obj = new T(this, std::move(args)...);
            processSceneObjectCreated(obj);
            return obj;
        }

        template<typename T>
        T* cloneSceneObject(const T* cloneFrom)
        {
            Q_ASSERT_X(cloneFrom->scene() == this, Q_FUNC_INFO, "Cannot clone object from a different scene!");
            T* obj = new T(cloneFrom);
            processSceneObjectCloned(obj);
            return obj;
        }

        void destroySceneObject(SceneObject* obj);

        SceneObject* rootObject() const;

    private:
        void processSceneObjectCreated(SceneObject* object);
        void processSceneObjectCloned(SceneObject* object);

        SceneObject* m_pRootObject;
    };
}

#endif // SCENE_H
