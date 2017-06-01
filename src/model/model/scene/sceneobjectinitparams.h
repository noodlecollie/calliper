#ifndef SCENEOBJECTINITPARAMS_H
#define SCENEOBJECTINITPARAMS_H

#include "model_global.h"

#include "rendersystem/interface-classes/definitions/publicrendermodeldefs.h"

namespace Model
{
    class Scene;

    class MODELSHARED_EXPORT SceneObjectInitParams
    {
    public:
        SceneObjectInitParams(Scene* parentScene, RenderSystem::PublicRenderModelDefs::ObjectId objectId);

        Scene* m_pScene;
        RenderSystem::PublicRenderModelDefs::ObjectId m_iObjectId;
    };
}

#endif // SCENEOBJECTINITPARAMS_H
