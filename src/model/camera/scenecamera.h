#ifndef SCENECAMERA_H
#define SCENECAMERA_H

#include "model_global.h"
#include "scene/sceneobject.h"
#include "cameralens.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT SceneCamera : public SceneObject
    {
        friend class Scene;
        Q_OBJECT
    public:
        virtual bool scalable() const override;

        CameraLens lens() const;
        void setLens(const CameraLens &lens);

        QMatrix4x4 worldToCameraMarix() const;

    protected:
        SceneCamera(const SceneObjectInitParams &initParams, SceneObject* parentObject);
        explicit SceneCamera(const SceneCamera* cloneFrom, const SceneObjectInitParams &initParams);
        virtual ~SceneCamera();

    private:
        void commonInit();

        CameraLens  m_Lens;
    };
}

#endif // SCENECAMERA_H
