#ifndef BASESCENE_H
#define BASESCENE_H

#include <QList>

class SceneObject;
class SceneCamera;
class MapScene;
class UIScene;
class MapDocument;

class BaseScene
{
public:
    enum SceneType
    {
        TypeBaseScene = 0,
        TypeMapScene,
        TypeUIScene
    };

    BaseScene(MapDocument* doc);
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

protected:
    SceneObject*    m_pRootObject;
    MapDocument*    m_pDocument;
};

#endif // BASESCENE_H
