#include "texturestore.h"

namespace NS_MODEL
{
    TextureStore::TextureStore()
        : m_iNextTextureId(1)
    {

    }

    TextureStore::~TextureStore()
    {
        using namespace Renderer;
        foreach ( OpenGLTexturePointer tex, m_TextureTable.values() )
        {
            tex->destroy();
        }
    }

    quint32 TextureStore::acquireNextTextureId()
    {
        Q_ASSERT_X(m_iNextTextureId + 1 != 0, Q_FUNC_INFO, "Texture ID counter overflow!");
        return m_iNextTextureId++;
    }

    Renderer::OpenGLTexturePointer TextureStore::getTexture(quint32 textureId) const
    {
        return m_TextureTable.value(textureId, Renderer::OpenGLTexturePointer());
    }

    Renderer::OpenGLTexturePointer TextureStore::operator ()(quint64 textureId) const
    {
        return getTexture(textureId);
    }

    Renderer::OpenGLTexturePointer TextureStore::createTexture(const QString &path)
    {
        using namespace Renderer;

        if ( m_TexturePathTable.contains(path) )
        {
            return getTexture(m_TexturePathTable.value(path));
        }

        return createTextureInternal(path, acquireNextTextureId());
    }

    Renderer::OpenGLTexturePointer TextureStore::createDefaultTexture(const QString &path)
    {
        using namespace Renderer;

        if ( m_TextureTable.contains(0) )
        {
            return m_TextureTable.value(0);
        }

        if ( m_TexturePathTable.contains(path) )
        {
            return getTexture(path);
        }

        return createTextureInternal(path, 0);
    }

    Renderer::OpenGLTexturePointer TextureStore::createTextureInternal(const QString &path, quint32 id)
    {
        using namespace Renderer;

        OpenGLTexturePointer texture = OpenGLTexturePointer::create(id, QImage(path).mirrored());
        texture->setPath(path);
        texture->create();
        m_TextureTable.insert(id, texture);
        m_TexturePathTable.insert(path, id);
        return texture;
    }

    quint32 TextureStore::getTextureId(const QString &path) const
    {
        return m_TexturePathTable.value(path, 0);
    }

    Renderer::OpenGLTexturePointer TextureStore::getTexture(const QString &path) const
    {
        return getTexture(getTextureId(path));
    }
}
