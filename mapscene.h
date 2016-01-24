#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QList>

class SceneCamera;
class SceneObject;
class MapDocument;
class BaseGrid;

class MapScene
{
    friend class BaseGrid;
public:
    MapScene(MapDocument* document);
    ~MapScene();

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

#endif // MAPSCENE_H
