#include "sceneobjectinitparams.h"

namespace Model
{
    SceneObjectInitParams::SceneObjectInitParams(Scene* parentScene, quint32 objectId)
        : m_pScene(parentScene), m_iObjectId(objectId)
    {

    }
}
