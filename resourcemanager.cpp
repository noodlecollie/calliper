#include "resourcemanager.h"
#include <QGuiApplication>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QtDebug>

static ResourceManager* g_pResourceManager = NULL;
ResourceManager* resourceManager()
{
    return g_pResourceManager;
}

void ResourceManager::initialise()
{
    Q_ASSERT(!g_pResourceManager);
    g_pResourceManager = new ResourceManager();
}

void ResourceManager::shutdown()
{
    Q_ASSERT(g_pResourceManager);
    delete g_pResourceManager;
    g_pResourceManager = NULL;
}

ResourceManager::ResourceManager()
{
    m_pSurface = new QOffscreenSurface();
    m_pSurface->setFormat(QSurfaceFormat::defaultFormat());
    m_pSurface->create();

    m_pBackgroundContext = new QOpenGLContext();
    m_pBackgroundContext->setScreen(QGuiApplication::screens().at(0));
    m_pBackgroundContext->setShareContext(QOpenGLContext::globalShareContext());

    bool success = m_pBackgroundContext->create();
    Q_ASSERT(success);

    qDebug() << "OpenGL format acquired:" << m_pBackgroundContext->format();
}

ResourceManager::~ResourceManager()
{
    m_pBackgroundContext->makeCurrent(m_pSurface);

    // Clean up any resources that require an active context here.

    m_pBackgroundContext->doneCurrent();
    delete m_pBackgroundContext;

    m_pSurface->destroy();
    delete m_pSurface;
}

QOpenGLContext* ResourceManager::backgroundContext() const
{
    return m_pBackgroundContext;
}

QOffscreenSurface* ResourceManager::surface() const
{
    return m_pSurface;
}

QOpenGLFunctions_4_1_Core* ResourceManager::functions() const
{
    return liveContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();
}

QOpenGLContext* ResourceManager::liveContext() const
{
    return m_pLiveContext ? m_pLiveContext : m_pBackgroundContext;
}

void ResourceManager::setLiveContext(QOpenGLContext *context)
{
    m_pLiveContext = context;
}
