#ifndef SCENE_H
#define SCENE_H

#include <QList>

class Camera;
class SceneObject;

class Scene
{
public:
    Scene();
    ~Scene();

    SceneObject* root() const;

    QList<Camera*> findCameras() const;
    QList<SceneObject*> findByName(const QString &name);

private:
    SceneObject*    m_pRootObject;
};

#endif // SCENE_H
