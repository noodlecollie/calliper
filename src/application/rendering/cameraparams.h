#ifndef CAMERAPARAMS_H
#define CAMERAPARAMS_H

#include <QMatrix4x4>
#include "boundingbox.h"

class CameraLens;
class HierarchicalObject;
class SceneCamera;

class CameraParams
{
public:
    explicit CameraParams();
    explicit CameraParams(const QMatrix4x4 w2c, const QMatrix4x4 proj, const HierarchicalObject* obj = nullptr,
                 const CameraLens* lens = nullptr, const BoundingBox &bounds = BoundingBox());
    explicit CameraParams(const SceneCamera* camera);

    const QMatrix4x4& worldToCameraMatrix() const;
    const QMatrix4x4& projectionMatrix() const;
    const HierarchicalObject* hierarchicalObject() const;
    const CameraLens* lens() const;
    const BoundingBox& globalBounds() const;

private:
    QMatrix4x4 m_matWorldToCameraMatrix;
    QMatrix4x4 m_matProjectionMatrix;
    const HierarchicalObject* m_pObject;
    const CameraLens* m_pLens;
    BoundingBox m_GlobalBounds;
};

#endif // CAMERAPARAMS_H
