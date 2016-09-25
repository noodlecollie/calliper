#include "rendererdrawparams.h"

namespace NS_RENDERER
{
    RendererDrawParams::RendererDrawParams(const QMatrix4x4 &worldToCameraMatrix, const QMatrix4x4 &projectionMatrix)
        : m_matWorldToCamera(worldToCameraMatrix), m_matProjection(projectionMatrix)
    {

    }

    RendererDrawParams::RendererDrawParams()
        : RendererDrawParams(QMatrix4x4(), QMatrix4x4())
    {

    }

    RendererDrawParams::RendererDrawParams(const RendererDrawParams &other)
        : m_matWorldToCamera(other.m_matWorldToCamera), m_matProjection(other.m_matProjection)
    {

    }

    const QMatrix4x4& RendererDrawParams::worldToCameraMatrix() const
    {
        return m_matWorldToCamera;
    }

    const QMatrix4x4& RendererDrawParams::projectionMatrix() const
    {
        return m_matProjection;
    }

    int RendererDrawParams::size() const
    {
        return 2 * 16 * sizeof(float);
    }
}
