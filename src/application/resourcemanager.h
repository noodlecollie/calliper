#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>
#include <QVector>
#include <QHash>
#include <QOpenGLTexture>

class QOffscreenSurface;
class QOpenGLContext;
class ShaderProgram;
class SimpleNumericFont;
class QOpenGLFramebufferObject;

class ResourceManager
{
public:
    static void initialise();
    static void shutdown();

    ResourceManager();
    ~ResourceManager();

    QOpenGLContext* backgroundContext() const;
    QOffscreenSurface* surface() const;
    void makeCurrent();
    void doneCurrent();

    // Acts upon the current context;
    QOpenGLFunctions_4_1_Core* functions() const;

    void setUpOpenGLResources();

    ShaderProgram* shader(int index) const;
    ShaderProgram* shader(const QString &name) const;
    int shaderIndex(const QString &name) const;

    // Returns the default texture if the path is not found.
    QOpenGLTexture* texture(const QString &path) const;

    SimpleNumericFont* numericFont() const;
    QOpenGLFramebufferObject* frameBuffer(const QSize &size);

private:
    typedef QHash<QString, QOpenGLTexture*> TextureTable;

    void setUpShaders();
    void setUpBuiltInTextures();
    void setUpInternalTextures();
    void addBuiltInTexture(const QString &path, QOpenGLTexture::Filter min, QOpenGLTexture::Filter max,
                           QOpenGLTexture::WrapMode s = QOpenGLTexture::Repeat, QOpenGLTexture::WrapMode t = QOpenGLTexture::Repeat);

    QOffscreenSurface*  m_pSurface;
    QOpenGLContext*     m_pBackgroundContext;

    QVector<ShaderProgram*> m_Shaders;
    TextureTable            m_Textures;

    QOpenGLTexture*     m_pDefaultTexture;
    SimpleNumericFont*  m_pNumericFont;
    QOpenGLFramebufferObject*   m_pFramebuffer;
};

ResourceManager* resourceManager();

#endif // RESOURCEMANAGER_H
