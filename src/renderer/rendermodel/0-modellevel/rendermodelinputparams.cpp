#include "rendermodelinputparams.h"

namespace NS_RENDERER
{
    RenderModelInputParams::RenderModelInputParams(int passIndex, quint32 shaderId, quint64 textureId, const QMatrix4x4 &modelToWorld,
                                                   GLenum drawMode, float drawWidth)
        : m_iPassIndex(passIndex),
          m_iShaderId(shaderId),
          m_iTextureId(textureId),
          m_matModelToWorld(modelToWorld),
          m_iDrawMode(drawMode),
          m_flDrawWidth(drawWidth)
    {

    }

    RenderModelInputParams::RenderModelInputParams(const RenderModelInputParams &other)
        : m_iPassIndex(other.m_iPassIndex),
          m_iShaderId(other.m_iShaderId),
          m_iTextureId(other.m_iTextureId),
          m_matModelToWorld(other.m_matModelToWorld),
          m_iDrawMode(other.m_iDrawMode),
          m_flDrawWidth(other.m_flDrawWidth)
    {

    }

    int RenderModelInputParams::passIndex() const
    {
        return m_iPassIndex;
    }

    quint32 RenderModelInputParams::shaderId() const
    {
        return m_iShaderId;
    }

    quint64 RenderModelInputParams::textureId() const
    {
        return m_iTextureId;
    }

    const QMatrix4x4& RenderModelInputParams::modelToWorldMatrix() const
    {
        return m_matModelToWorld;
    }

    GLenum RenderModelInputParams::drawMode() const
    {
        return m_iDrawMode;
    }

    float RenderModelInputParams::drawWidth() const
    {
        return m_flDrawWidth;
    }
}
