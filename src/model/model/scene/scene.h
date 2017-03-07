#ifndef SCENE_H
#define SCENE_H

#include "model_global.h"
#include "model/scene/sceneobject.h"
#include <QHash>
#include "sceneobjectinitparams.h"
#include "model/stores/texturestore.h"
#include "model/stores/shaderstore.h"
#include "model/stores/materialstore.h"
#include "model/scenerenderer/irenderpassclassifier.h"

namespace Model
{
    class MODELSHARED_EXPORT Scene : public QObject,
                                     public IRenderPassClassifier
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

        SceneObject* rootObject() const;

        virtual int classify(quint32 objectId) const override;

    signals:
        void objectCreated(SceneObject*);
        void objectDestroyed(SceneObject*);

    private:
        void processSceneObjectCreated(SceneObject* object);
        void processSceneObjectCloned(SceneObject* object);
        quint32 acquireNextObjectId();
        void addObjectToTable(SceneObject* object);
        void removeObjectFromTable(SceneObject* object);
        void deleteObjectsRecursive(SceneObject* object);

        quint32 m_iObjectIdCounter;
        SceneObject* m_pRootObject;
        QHash<quint32, SceneObject*> m_ObjectTable;
    };
}

#endif // SCENE_H
