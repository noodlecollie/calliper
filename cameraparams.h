#ifndef CAMERAPARAMS_H
#define CAMERAPARAMS_H

#include <QMatrix4x4>

class CameraLens;
class HierarchicalObject;
class SceneCamera;

class CameraParams
{
public:
    CameraParams(const QMatrix4x4 w2c, const QMatrix4x4 proj, const HierarchicalObject* obj = NULL,
                 const CameraLens* lens = NULL);
    CameraParams(const SceneCamera* camera);

    const QMatrix4x4& worldToCameraMatrix() const;
    const QMatrix4x4& projectionMatrix() const;
    const HierarchicalObject* hierarchicalObject() const;
    const CameraLens* lens() const;

private:
    QMatrix4x4 m_matWorldToCameraMatrix;
    QMatrix4x4 m_matProjectionMatrix;
    const HierarchicalObject* m_pObject;
    const CameraLens* m_pLens;
};

#endif // CAMERAPARAMS_H
