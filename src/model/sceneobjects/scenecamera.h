#ifndef SCENECAMERA_H
#define SCENECAMERA_H

#include "model_global.h"
#include "sceneobject.h"
#include "cameralens.h"
#include <QScopedPointer>
#include "ray3d.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT SceneCamera : public SceneObject
    {
        Q_OBJECT
        friend class BaseScene;
    public:
        CameraLens* lens() const;
        QVector3D forwardVector() const;

        // The world -> camera transform is parentToLocal().

        virtual bool scalable() const;

        // The translation is defined from p0 to p1, along the plane that is parallel to the
        // camera view plane and at the specified distance in front of where the camera is pointing.
        QVector3D worldTranslation(const QPoint &p0, const QPoint &p1, const QSize &viewSize, float distFromCamera) const;

        // Given a point on the screen, returns the direction of a ray cast from the camera origin that goes through that point.
        // This is a world direction.
        QVector3D frustumDirection(const QPoint &p, const QSize &viewSize) const;
        Ray3D frustumRay(const QPoint &p, const QSize &viewSize) const;

        virtual bool serialiseToJson(QJsonObject &obj) const;
        virtual QString serialiseIdentifier() const;

    protected:
        explicit SceneCamera(BaseScene* scene, SceneObject* parent);
        SceneCamera(BaseScene* scene, const QJsonObject &serialisedData, SceneObject* parent);
        SceneCamera(const SceneCamera &cloneFrom);
        virtual ~SceneCamera();
        virtual void rebuildLocalToParent() const;
        virtual void clampAngles();

    private:
        void initDefaults();

        QScopedPointer<CameraLens>  m_pLens;
    };
}

#endif // SCENECAMERA_H
