#ifndef BASESCENE_H
#define BASESCENE_H

#include <QObject>

class SceneObject;
class SceneCamera;
class MapScene;
class UIScene;
class MapDocument;

class BaseScene : public QObject
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

    explicit BaseScene(MapDocument* doc);
    virtual ~BaseScene();
    virtual SceneType type() const;

    // Convenience
    MapScene* mapScene();
    const MapScene* mapScene() const;
    UIScene* uiScene();
    const UIScene* uiScene() const;

    SceneObject* root() const;
    MapDocument* document() const;

    QList<SceneObject*> findByName(const QString &name);
    QList<SceneCamera*> findCameras() const;

    template<typename T, typename... Args>
    T* createSceneObject(Args... args)
    {
        T* obj = new T(std::move(args)...);
        Q_ASSERT(qobject_cast<SceneObject*>(obj));

        if ( !obj->parentObject() )
            setRoot(obj);

        obj->m_pScene = this;
        emit sceneObjectCreated(obj);
        return obj;
    }

    SceneObject* unserialiseSceneObject(const QJsonObject &serialisedData, SceneObject* parent);
    void destroySceneObject(SceneObject* obj);

signals:
    void sceneObjectCreated(SceneObject* object);
    void subtreeDestroyed(SceneObject* object);

protected:
    void setRoot(SceneObject* root);

    SceneObject*    m_pRootObject;
};

#endif // BASESCENE_H
