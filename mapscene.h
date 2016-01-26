#ifndef SCENE_H
#define SCENE_H

#include "basescene.h"

class MapDocument;
class MapGrid;

class MapScene : public BaseScene
{
    friend class MapGrid;
public:
    MapScene(MapDocument* document);
    virtual ~MapScene();
    virtual SceneType type() const;

    MapGrid* grid() const;

private:
    MapGrid*       m_pGrid;
};

#endif // SCENE_H
