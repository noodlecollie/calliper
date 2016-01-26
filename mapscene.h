#ifndef SCENE_H
#define SCENE_H

#include "basescene.h"

class MapDocument;
class BaseGrid;

class MapScene : public BaseScene
{
    friend class BaseGrid;
public:
    MapScene(MapDocument* document);
    virtual ~MapScene();
    virtual SceneType type() const;

    BaseGrid* grid() const;

private:
    BaseGrid*       m_pGrid;
};

#endif // SCENE_H
