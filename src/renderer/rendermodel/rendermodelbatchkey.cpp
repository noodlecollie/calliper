#include "rendermodelbatchkey.h"
#include <QByteArray>
#include <QDataStream>

namespace NS_RENDERER
{
    uint qHash(const RenderModelBatchKey &key, uint seed)
    {
        QByteArray arr;

        {
            QDataStream stream(&arr, QIODevice::WriteOnly);
            stream << key.shaderStoreId()
                   << key.textureId()
                   << key.drawMode()
                   << key.drawWidth();

            stream.writeBytes((const char*)key.modelToWorldMatrix().constData(), 16 * sizeof(float));
        }

        return qHash(arr, seed);
    }

    RenderModelBatchKey::RenderModelBatchKey(quint16 shaderStoreId, quint32 textureId, const QMatrix4x4 &mat,
                                             GLenum drawMode, float drawWidth)
        : m_iShaderStoreId(shaderStoreId), m_iTextureId(textureId), m_iDrawMode(drawMode), m_flDrawWidth(drawWidth),
          m_matModelToWorld(mat)
    {

    }

    RenderModelBatchKey::RenderModelBatchKey(const RenderModelBatchKey &other)
        : m_iShaderStoreId(other.m_iShaderStoreId), m_iTextureId(other.m_iTextureId), m_iDrawMode(other.m_iDrawMode),
          m_flDrawWidth(other.m_flDrawWidth), m_matModelToWorld(other.m_matModelToWorld)
    {

    }

    quint16 RenderModelBatchKey::shaderStoreId() const
    {
        return m_iShaderStoreId;
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

    const QMatrix4x4& RenderModelBatchKey::modelToWorldMatrix() const
    {
        return m_matModelToWorld;
    }
}
