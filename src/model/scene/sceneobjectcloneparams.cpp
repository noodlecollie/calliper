#include "sceneobjectcloneparams.h"

namespace NS_MODEL
{
    SceneObjectCloneParams::SceneObjectCloneParams(Scene* parentScene, quint32 objectId, const SceneObject* cloneFrom)
        : m_pScene(parentScene), m_iObjectId(objectId), m_pCloneFrom(cloneFrom)
    {

    }
}
