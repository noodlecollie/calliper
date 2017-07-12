#include "framedrawparams.h"

namespace RenderSystem
{
    FrameDrawParams::FrameDrawParams()
        : m_nRenderMode(RenderSystem::ShaderDefs::BarebonesRenderMode),
          m_matWorldToCamera(),
          m_matProjection(),
          m_colBackground(QColor::fromRgb(0x000000FF)),
          m_vecDirectionalLight(1,1,1)
    {

    }

    RenderSystem::ShaderDefs::RenderMode FrameDrawParams::renderMode() const
    {
        return m_nRenderMode;
    }

    void FrameDrawParams::setRenderMode(RenderSystem::ShaderDefs::RenderMode mode)
    {
        m_nRenderMode = mode;
    }

    const QMatrix4x4& FrameDrawParams::worldToCameraMatrix() const
    {
        return m_matWorldToCamera;
    }

    void FrameDrawParams::setWorldToCameraMatrix(const QMatrix4x4 &mat)
    {
        m_matWorldToCamera = mat;
    }

    const QMatrix4x4& FrameDrawParams::projectionMatrix() const
    {
        return m_matProjection;
    }

    void FrameDrawParams::setProjectionMatrix(const QMatrix4x4 &mat)
    {
        m_matProjection = mat;
    }

    QColor FrameDrawParams::backgroundColor() const
    {
        return m_colBackground;
    }

    void FrameDrawParams::setBackgroundColor(const QColor &col)
    {
        m_colBackground = col;
    }

    QVector3D FrameDrawParams::directionalLight() const
    {
        return m_vecDirectionalLight;
    }

    void FrameDrawParams::setDirectionalLight(const QVector3D &dir)
    {
        m_vecDirectionalLight = dir;
    }
}
