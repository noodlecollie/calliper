#include "resourcemanager.h"
#include <QGuiApplication>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QtDebug>
#include "minimumshader.h"
#include "minimumtexturedshader.h"
#include <QOpenGLTexture>
#include "unlittextureshader.h"

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
    setUpShaders();
    setUpBuiltInTextures();
}

void ResourceManager::setUpShaders()
{
    MinimumShader* minSh = new MinimumShader();
    minSh->construct();
    m_Shaders.append(minSh);

    MinimumTexturedShader* minTSh = new MinimumTexturedShader();
    minTSh->construct();
    m_Shaders.append(minTSh);

    UnlitTextureShader* unlTSh = new UnlitTextureShader();
    unlTSh->construct();
    m_Shaders.append(unlTSh);
}

void ResourceManager::setUpBuiltInTextures()
{
    // Default texture - returned if any other texture is not found.
    m_pDefaultTexture = new QOpenGLTexture(QImage(":/textures/error.png").mirrored());
    m_pDefaultTexture->setMinificationFilter(QOpenGLTexture::NearestMipMapNearest);
    m_pDefaultTexture->setMagnificationFilter(QOpenGLTexture::NearestMipMapNearest);
    m_pDefaultTexture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    m_pDefaultTexture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    m_Textures.insert(QString(), m_pDefaultTexture);

    // Other random ones.
    QString path1(":/textures/test.png");
//    QOpenGLTexture* t1 = new QOpenGLTexture(QImage(path1).mirrored());
    QOpenGLTexture* t1 = new QOpenGLTexture(QOpenGLTexture::Target2D);
    t1->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    t1->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    t1->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    t1->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    t1->setMipLevels(2);
    t1->setData(QImage(path1).mirrored());
    m_Textures.insert(path1.mid(1).remove(".png"), t1);

    QString path2(":/textures/uvsample.png");
    QOpenGLTexture* t2 = new QOpenGLTexture(QImage(path2).mirrored());
    t2->setMinificationFilter(QOpenGLTexture::Nearest/*MipMapNearest*/);
    t2->setMagnificationFilter(QOpenGLTexture::Nearest/*MipMapNearest*/);
    t2->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    t2->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    m_Textures.insert(path2.mid(1).remove(".png"), t2);
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

    qDeleteAll(m_Textures);
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

ShaderProgram* ResourceManager::shader(int index) const
{
    return m_Shaders.at(index);
}

ShaderProgram* ResourceManager::shader(const QString &name) const
{
    for ( int i = 0; i < m_Shaders.count(); i++ )
    {
        if ( m_Shaders.at(i)->objectName() == name )
            return m_Shaders.at(i);
    }

    return NULL;
}

QOpenGLTexture* ResourceManager::texture(const QString &path) const
{
    QOpenGLTexture* tex = m_Textures.value(path, NULL);
    return tex ? tex : m_pDefaultTexture;
}
