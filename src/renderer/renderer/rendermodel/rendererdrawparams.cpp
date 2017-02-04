#include "rendererdrawparams.h"

namespace Renderer
{
    RendererDrawParams::RendererDrawParams()
    {

    }

    const QMatrix4x4& RendererDrawParams::worldToCameraMatrix() const
    {
        return m_matWorldToCamera;
    }

    void RendererDrawParams::setWorldToCameraMatrix(const QMatrix4x4 &mat)
    {
        m_matWorldToCamera = mat;
    }

    const QMatrix4x4& RendererDrawParams::projectionMatrix() const
    {
        return m_matProjection;
    }

    void RendererDrawParams::setProjectionMatrix(const QMatrix4x4 &mat)
    {
        m_matProjection = mat;
    }

    const QVector3D& RendererDrawParams::directionalLight() const
    {
        return m_vecDirectionalLight;
    }

    void RendererDrawParams::setDirectionalLight(const QVector3D &vec)
    {
        m_vecDirectionalLight = vec;
    }
}
