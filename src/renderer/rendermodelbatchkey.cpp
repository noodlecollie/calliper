#include "rendermodelbatchkey.h"
#include <QByteArray>
#include <QDataStream>

namespace NS_RENDERER
{
    RenderModelBatchKey::RenderModelBatchKey(quint16 shaderId, quint32 textureId, GLenum drawMode, float drawWidth)
        : m_iShaderId(shaderId), m_iTextureId(textureId), m_iDrawMode(drawMode), m_flDrawWidth(drawWidth)
    {

    }

    RenderModelBatchKey::RenderModelBatchKey(const RenderModelBatchKey &other)
        : m_iShaderId(other.m_iShaderId), m_iTextureId(other.m_iTextureId), m_iDrawMode(other.m_iDrawMode),
          m_flDrawWidth(other.m_flDrawWidth)
    {

    }

    quint16 RenderModelBatchKey::shaderId() const
    {
        return m_iShaderId;
    }

    quint32 RenderModelBatchKey::textureId() const
    {
        return m_iTextureId;
    }

    GLenum RenderModelBatchKey::drawMode() const
    {
        return m_iDrawMode;
    }

    float RenderModelBatchKey::drawWidth() const
    {
        return m_flDrawWidth;
    }
}
