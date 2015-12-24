#include "resourcemanager.h"
#include <QGuiApplication>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QtDebug>
#include "minimumshader.h"

static ResourceManager* g_pResourceManager = NULL;
ResourceManager* resourceManager()
{
    return g_pResourceManager;
}

void ResourceManager::initialise()
{
    Q_ASSERT(!g_pResourceManager);
    new ResourceManager();
}

void ResourceManager::shutdown()
{
    Q_ASSERT(g_pResourceManager);
    delete g_pResourceManager;
    g_pResourceManager = NULL;
}

ResourceManager::ResourceManager()
{
    g_pResourceManager = this;
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

void ResourceManager::setUpOpenGLResources()
{
    MinimumShader* minSh = new MinimumShader();
    minSh->construct();
    m_Shaders.append(minSh);
}

void ResourceManager::makeCurrent()
{
    bool success = m_pBackgroundContext->makeCurrent(m_pSurface);
    Q_ASSERT(success);
    setLiveContext(m_pBackgroundContext);
}

void ResourceManager::doneCurrent()
{
    setLiveContext(NULL);
    m_pBackgroundContext->doneCurrent();
}

ResourceManager::~ResourceManager()
{
    makeCurrent();

    qDeleteAll(m_Shaders);

    doneCurrent();

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

ShaderProgram* ResourceManager::minimumShader() const
{
    Q_ASSERT(m_Shaders.count() > 0);
    return m_Shaders.at(0);
}
