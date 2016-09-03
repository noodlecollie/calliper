#include "texturestore.h"

namespace NS_RENDERER
{
    TextureStore* g_pGlobalTextureStore = NULL;

    TextureStore* TextureStore::getTextureStore()
    {
        return g_pGlobalTextureStore;
    }

    TextureStore::TextureStore()
        : m_iNextTextureId(1),
          m_pErrorTexture()
    {
        Q_ASSERT_X(!g_pGlobalTextureStore, Q_FUNC_INFO, "Texture store singleton already created!");

        g_pGlobalTextureStore = this;
        addBuiltInTextures();
    }

    TextureStore::~TextureStore()
    {
        g_pGlobalTextureStore = NULL;
    }

    quint64 TextureStore::createTexture(const QImage &image, const QString &path)
    {
        if ( path.isNull() || path.isEmpty() )
        {
            Q_ASSERT_X(false, Q_FUNC_INFO, "Texture must have a path!");
            return 0;
        }

        if ( m_TextureNameTable.contains(path) )
        {
            Q_ASSERT_X(false, Q_FUNC_INFO, "Texture with given path already exists!");
            return 0;
        }

        OpenGLTexturePointer texture(new OpenGLTexture(image));

        quint64 newId = nextTextureId();
        Q_ASSERT_X(newId != 0, Q_FUNC_INFO, "We should never get a new texture ID of 0!");
        texture->m_iId = newId;
        texture->setPath(path);

        m_TextureTable.insert(newId, texture);
        m_TextureNameTable.insert(path, newId);

        return newId;
    }

    quint64 TextureStore::nextTextureId()
    {
        quint64 next = m_iNextTextureId++;
        Q_ASSERT_X(m_iNextTextureId != 0, Q_FUNC_INFO, "This shouldn't overflow until at least 500,000 years' time, what the fuck did you do");
        return next;
    }

    void TextureStore::addBuiltInTextures()
    {
        // First of all create the default texture for when something's not found.
        createErrorTexture();

        // Then the others.
        addBuiltInTexture(":/textures/obsolete");
    }

    void TextureStore::addBuiltInTexture(const QString &path)
    {
        createTexture(QImage(path), QString(path).remove(":/textures/"));
    }

    void TextureStore::createErrorTexture()
    {
        // ID and path will default to 0 and blank.
        m_pErrorTexture = OpenGLTexturePointer(new OpenGLTexture(QImage(":/textures/_ERROR_")));
    }

    OpenGLTexturePointer TextureStore::texture(quint64 id) const
    {
        return m_TextureTable.value(id, m_pErrorTexture);
    }

    OpenGLTexturePointer TextureStore::texture(const QString &path) const
    {
        return texture(textureId(path));
    }

    quint64 TextureStore::textureId(const QString &path) const
    {
        return m_TextureNameTable.value(path, 0);
    }

    void TextureStore::initialise()
    {
        addBuiltInTextures();
    }
}
