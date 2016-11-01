#ifndef UISCENE_H
#define UISCENE_H

#include "basescene.h"

class MapDocument;

class UIScene : public BaseScene
{
public:
    UIScene(MapDocument* document);
    virtual ~UIScene();
    virtual SceneType type() const;

private:
    MapDocument*    m_pDocument;
};

#endif // UISCENE_H
