#ifndef SCENEOBJECTCLONEPARAMS_H
#define SCENEOBJECTCLONEPARAMS_H

#include "model_global.h"

namespace NS_MODEL
{
    class Scene;
    class SceneObject;

    class SceneObjectCloneParams
    {
    public:
        SceneObjectCloneParams(Scene* parentScene, quint32 objectId, const SceneObject* cloneFrom);

        Scene* m_pScene;
        quint32 m_iObjectId;
        const SceneObject* m_pCloneFrom;
    };
}

#endif // SCENEOBJECTCLONEPARAMS_H
