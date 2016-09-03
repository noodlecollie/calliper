#ifndef BASESCENE_H
#define BASESCENE_H

#include "model_global.h"
#include <QObject>
#include <QSet>

//#define LOG_SCENE_OBJECT_LIFETIMES

#ifdef LOG_SCENE_OBJECT_LIFETIMES
#include <QtDebug>
#endif

namespace NS_MODEL
{
    class SceneObject;
    class SceneCamera;
    class MapScene;
    class UIScene;
    class MapDocument;

    class MODELSHARED_EXPORT BaseScene : public QObject
    {
        Q_OBJECT
        friend class SceneObject;
    public:
        enum SceneType
        {
            TypeBaseScene = 0,
            TypeMapScene,
            TypeUIScene
        };

        enum SceneTypeFlag
        {
            MapSceneFlag = 0x1,
            UISceneFlag = 0x2,
        };

        explicit BaseScene(MapDocument* doc);
        virtual ~BaseScene();
        virtual SceneType type() const;

        // Convenience
        MapScene* mapScene();
        const MapScene* mapScene() const;
        UIScene* uiScene();
        const UIScene* uiScene() const;

        SceneObject* root() const;
        SceneObject* generalObjectsNode() const;
        void clear();
        MapDocument* document() const;

        QList<SceneObject*> findByName(const QString &name);
        QList<SceneCamera*> findCameras() const;

        template<typename T, typename... Args>
        T* createSceneObject(Args... args)
        {
            T* obj = new T(this, std::move(args)...);
            Q_ASSERT(qobject_cast<SceneObject*>(obj));
            Q_ASSERT(obj->parentObject());

    #ifdef LOG_SCENE_OBJECT_LIFETIMES
            qDebug() << "Scene object" << obj << "created";
    #endif

            emit sceneObjectCreated(obj);
            return obj;
        }

        template<typename T>
        T* cloneSceneObject(const T* cloneFrom)
        {
            Q_ASSERT(cloneFrom);
            Q_ASSERT(cloneFrom->metaObject()->className() == T::staticMetaObject.className());
            Q_ASSERT(cloneFrom->m_pScene == this);
            T* obj = new T(*cloneFrom);
            Q_ASSERT(qobject_cast<SceneObject*>(obj));

    #ifdef LOG_SCENE_OBJECT_LIFETIMES
            qDebug() << "Scene object" << cloneFrom << "cloned.";
    #endif

            emit sceneObjectCreated(obj);
            return obj;
        }

        void destroySceneObject(SceneObject* obj);

    signals:
        void sceneObjectCreated(SceneObject* object);
        void subtreeDestroyed(SceneObject* object);

    protected:
        SceneObject*        m_pRootObject;
        SceneObject*        m_pInternalObjects;
        SceneObject*        m_pGeneralObjects;

    private:
        void createRoot();
        void destroyRoot();
        void processAddObject(SceneObject* obj);
        void processRemoveObject(SceneObject* obj);
    };
}

#endif // BASESCENE_H
