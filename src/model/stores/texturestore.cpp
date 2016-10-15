#include "texturestore.h"

namespace NS_MODEL
{
    TextureStore::TextureStore()
        : m_iNextTextureId(1)
    {

    }

    TextureStore::~TextureStore()
    {

    }

    quint32 TextureStore::acquireNextTextureId()
    {
        Q_ASSERT_X(m_iNextTextureId + 1 != 0, Q_FUNC_INFO, "Texture ID counter overflow!");
        return m_iNextTextureId++;
    }

    NS_RENDERER::OpenGLTexturePointer TextureStore::getTexture(quint32 textureId) const
    {
        return m_TextureTable.value(textureId, NS_RENDERER::OpenGLTexturePointer());
    }

    NS_RENDERER::OpenGLTexturePointer TextureStore::operator ()(quint64 textureId) const
    {
        return getTexture(textureId);
    }

    NS_RENDERER::OpenGLTexturePointer TextureStore::createTexture(const QString &path, QOpenGLTexture::Target target)
    {
        using namespace NS_RENDERER;

        if ( m_TexturePathTable.contains(path) )
        {
            return getTexture(m_TexturePathTable.value(path));
        }

        quint32 id = acquireNextTextureId();
        OpenGLTexturePointer texture = OpenGLTexturePointer::create(id, target);
        m_TextureTable.insert(id, texture);
        m_TexturePathTable.insert(path, id);
        return texture;
    }

    quint32 TextureStore::getTextureId(const QString &path) const
    {
        return m_TexturePathTable.value(path, 0);
    }

    NS_RENDERER::OpenGLTexturePointer TextureStore::getTexture(const QString &path) const
    {
        return getTexture(getTextureId(path));
    }
}
