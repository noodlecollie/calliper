#include "cameraparams.h"
#include "hierarchicalobject.h"
#include "cameralens.h"
#include "scenecamera.h"

CameraParams::CameraParams() :
    m_matWorldToCameraMatrix(), m_matProjectionMatrix(), m_pObject(NULL), m_pLens(NULL)
{

}

CameraParams::CameraParams(const QMatrix4x4 w2c, const QMatrix4x4 proj,
                           const HierarchicalObject *obj, const CameraLens *lens) :
    m_matWorldToCameraMatrix(w2c), m_matProjectionMatrix(proj), m_pObject(obj), m_pLens(lens)
{

}

CameraParams::CameraParams(const SceneCamera *camera) :
    m_matWorldToCameraMatrix(camera->rootToLocal()), m_matProjectionMatrix(camera->lens()->projectionMatrix()),
    m_pObject(camera), m_pLens(camera->lens())
{

}

const QMatrix4x4& CameraParams::worldToCameraMatrix() const
{
    return m_matWorldToCameraMatrix;
}

const QMatrix4x4& CameraParams::projectionMatrix() const
{
    return m_matProjectionMatrix;
}

const HierarchicalObject* CameraParams::hierarchicalObject() const
{
    return m_pObject;
}

const CameraLens* CameraParams::lens() const
{
    return m_pLens;
}
