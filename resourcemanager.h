#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>
#include <QVector>
#include <QHash>

class QOffscreenSurface;
class QOpenGLContext;
class ShaderProgram;
class QOpenGLTexture;

class ResourceManager
{
public:
    static void initialise();
    static void shutdown();

    ResourceManager();
    ~ResourceManager();

    QOpenGLContext* backgroundContext() const;
    QOffscreenSurface* surface() const;
    QOpenGLFunctions_4_1_Core* functions() const;
    void makeCurrent();
    void doneCurrent();

    void setLiveContext(QOpenGLContext* context);
    QOpenGLContext* liveContext() const;

    void setUpOpenGLResources();

    ShaderProgram* shader(int index) const;
    ShaderProgram* shader(const QString &name) const;

    // Returns the default texture if the path is not found.
    QOpenGLTexture* texture(const QString &path) const;

private:
    typedef QHash<QString, QOpenGLTexture*> TextureTable;

    void setUpShaders();
    void setUpBuiltInTextures();

    QOffscreenSurface*  m_pSurface;
    QOpenGLContext*     m_pBackgroundContext;
    QOpenGLContext*     m_pLiveContext;

    QVector<ShaderProgram*> m_Shaders;
    TextureTable            m_Textures;

    QOpenGLTexture* m_pDefaultTexture;
};

ResourceManager* resourceManager();

#endif // RESOURCEMANAGER_H