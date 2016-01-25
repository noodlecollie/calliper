#ifndef SCENE_H
#define SCENE_H

#include <QList>

class SceneCamera;
class SceneObject;
class MapDocument;
class BaseGrid;

class Scene
{
    friend class BaseGrid;
public:
    Scene(MapDocument* document);
    ~Scene();

    MapDocument* document() const;
    SceneObject* root() const;
    BaseGrid* grid() const;

    QList<SceneCamera*> findCameras() const;
    QList<SceneObject*> findByName(const QString &name);

private:
    SceneObject*    m_pRootObject;
    MapDocument*    m_pDocument;
    BaseGrid*       m_pGrid;
};

#endif // SCENE_H
