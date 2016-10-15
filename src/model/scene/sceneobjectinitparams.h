#ifndef SCENEOBJECTINITPARAMS_H
#define SCENEOBJECTINITPARAMS_H

#include "model_global.h"

namespace NS_MODEL
{
    class Scene;

    class MODELSHARED_EXPORT SceneObjectInitParams
    {
    public:
        SceneObjectInitParams(Scene* parentScene, quint32 objectId);

        Scene* m_pScene;
        quint32 m_iObjectId;
    };
}

#endif // SCENEOBJECTINITPARAMS_H
