#ifndef SCENE_H
#define SCENE_H

#include "model_global.h"
#include <QObject>

namespace NS_MODEL
{
    class SceneObject;

    class Scene : public QObject
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
            T* obj = new T(cloneFrom);
            processSceneObjectCloned(obj);
            return obj;
        }

        void destroySceneObject(SceneObject* obj);

    private:
        void processSceneObjectCreated(SceneObject* object);
        void processSceneObjectCloned(SceneObject* object);
    };
}

#endif // SCENE_H
