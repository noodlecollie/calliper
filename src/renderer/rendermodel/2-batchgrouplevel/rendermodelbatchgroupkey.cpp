#include "rendermodelbatchgroupkey.h"

namespace NS_RENDERER
{
    RenderModelBatchGroupKey::RenderModelBatchGroupKey(quint16 shaderId, quint16 textureId, GLenum drawMode, float drawWidth)
        : m_iShaderId(shaderId), m_iTextureId(textureId), m_iDrawMode(drawMode), m_flDrawWidth(drawWidth)
    {

    }

    quint16 RenderModelBatchGroupKey::shaderId() const
    {
        return m_iShaderId;
    }

    quint32 RenderModelBatchGroupKey::textureId() const
    {
        return m_iTextureId;
    }

    GLenum RenderModelBatchGroupKey::drawMode() const
    {
        return m_iDrawMode;
    }

    float RenderModelBatchGroupKey::drawWidth() const
    {
        return m_flDrawWidth;
    }
}
