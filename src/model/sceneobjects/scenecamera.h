#ifndef SCENECAMERA_H
#define SCENECAMERA_H

#include "model_global.h"
#include "scene/sceneobject.h"
#include "camera/cameralens.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT SceneCamera : public SceneObject
    {
        friend class Scene;
    public:
        virtual bool scalable() const override;

        CameraLens lens() const;
        void setLens(const CameraLens &lens);

    protected:
        SceneCamera(Scene* parentScene, SceneObject* parentObject);
        explicit SceneCamera(const SceneCamera* cloneFrom);
        virtual ~SceneCamera();

    private:
        void commonInit();

        CameraLens  m_Lens;
    };
}

#endif // SCENECAMERA_H
