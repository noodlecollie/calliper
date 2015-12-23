#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>

class QOffscreenSurface;
class QOpenGLContext;

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

    void setLiveContext(QOpenGLContext* context);
    QOpenGLContext* liveContext() const;

private:
    QOffscreenSurface*  m_pSurface;
    QOpenGLContext*     m_pBackgroundContext;
    QOpenGLContext*     m_pLiveContext;
};

ResourceManager* resourceManager();

#endif // RESOURCEMANAGER_H
