#ifndef SCENE_H
#define SCENE_H

#include <QList>

class Camera;
class SceneObject;
class MapDocument;

class Scene
{
public:
    Scene(MapDocument* document);
    ~Scene();

    MapDocument* document() const;
    SceneObject* root() const;

    QList<Camera*> findCameras() const;
    QList<SceneObject*> findByName(const QString &name);

private:
    SceneObject*    m_pRootObject;
    MapDocument*    m_pDocument;
};

#endif // SCENE_H
