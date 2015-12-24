#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>
#include <QVector>

class QOffscreenSurface;
class QOpenGLContext;
class ShaderProgram;

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

    ShaderProgram* minimumShader() const;

    void setUpOpenGLResources();

    ShaderProgram* shader(int index) const;
    ShaderProgram* shader(const QString &name) const;

private:
    QOffscreenSurface*  m_pSurface;
    QOpenGLContext*     m_pBackgroundContext;
    QOpenGLContext*     m_pLiveContext;

    QVector<ShaderProgram*> m_Shaders;
};

ResourceManager* resourceManager();

#endif // RESOURCEMANAGER_H
