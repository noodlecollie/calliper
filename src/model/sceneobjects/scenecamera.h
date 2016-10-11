#ifndef SCENECAMERA_H
#define SCENECAMERA_H

#include "model_global.h"
#include "scene/sceneobject.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT SceneCamera : public SceneObject
    {
        friend class Scene;
    public:

    protected:
        SceneCamera(Scene* parentScene, SceneObject* parentObject);
        explicit SceneCamera(const SceneCamera* cloneFrom);
        virtual ~SceneCamera();

    private:
        void commonInit();
    };
}

#endif // SCENECAMERA_H
