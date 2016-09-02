#include "rendermodelbatchkey.h"
#include <QByteArray>
#include <QDataStream>

uint qHash(NS_RENDERER::RenderModelBatchKey &key, uint seed)
{
    QByteArray arr;

    {
        QDataStream stream(&arr, QIODevice::WriteOnly);
        stream << key.shaderId()
               << key.textureId()
               << key.drawMode()
               << key.drawWidth();
    }

    return qHash(arr, seed);
}

namespace NS_RENDERER
{
    RenderModelBatchKey::RenderModelBatchKey(quint16 shaderId, quint32 textureId, GLenum drawMode, float drawWidth)
        : m_iShaderId(shaderId), m_iTextureId(textureId), m_iDrawMode(drawMode), m_flDrawWidth(drawWidth)
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
