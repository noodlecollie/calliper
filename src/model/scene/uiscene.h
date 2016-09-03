#ifndef UISCENE_H
#define UISCENE_H

#include "model_global.h"
#include "basescene.h"

namespace NS_MODEL
{
    class MapDocument;

    class MODELSHARED_EXPORT UIScene : public BaseScene
    {
    public:
        UIScene(MapDocument* document);
        virtual ~UIScene();
        virtual SceneType type() const;

    private:
        MapDocument*    m_pDocument;
    };
}

#endif // UISCENE_H
