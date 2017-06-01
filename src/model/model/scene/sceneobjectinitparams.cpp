#include "sceneobjectinitparams.h"

namespace Model
{
    SceneObjectInitParams::SceneObjectInitParams(Scene* parentScene, RenderSystem::RenderModelDefs::ObjectId objectId)
        : m_pScene(parentScene),
          m_iObjectId(objectId)
    {

    }
}
