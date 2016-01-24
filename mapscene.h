#ifndef MAPSCENE_H
#define MAPSCENE_H

#include "basescene.h"

class MapDocument;
class BaseGrid;

class MapScene : public BaseScene
{
    friend class BaseGrid;
public:
    MapScene(MapDocument* document);
    virtual ~MapScene();
    virtual SceneType sceneType() const;

    MapDocument* document() const;
    BaseGrid* grid() const;

private:
    MapDocument*    m_pDocument;
    BaseGrid*       m_pGrid;
};

#endif // MAPSCENE_H
