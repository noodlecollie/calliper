#include "rendermodelbatchgroupkey.h"
#include <QByteArray>
#include <QDataStream>

namespace Renderer
{
    uint qHash(const RenderModelBatchGroupKey &key, uint seed)
    {
        QByteArray arr;

        {
            QDataStream stream(&arr, QIODevice::WriteOnly);
            stream << key.shaderId() << key.textureId() << key.drawMode() << key.drawWidth();
        }

        return qHash(arr, seed);
    }

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
