#include "resourcemanager.h"
#include <QGuiApplication>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QtDebug>
#include "shaders.h"
#include <QOpenGLTexture>
#include "simplenumericfont.h"
#include <QOpenGLFramebufferObject>

#define MIPMAPS_DISABLED

#ifdef MIPMAPS_DISABLED
#define MIPMAP(_x) QOpenGLTexture::Linear
#else
#define MIPMAP(_x) (_x)
#endif

static ResourceManager* g_pResourceManager = nullptr;
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
    g_pResourceManager = nullptr;
}

ResourceManager::ResourceManager()
{
    g_pResourceManager = this;
    m_pFramebuffer = nullptr;
    m_pSurface = new QOffscreenSurface();
    m_pSurface->setFormat(QSurfaceFormat::defaultFormat());
    m_pSurface->create();

    m_pBackgroundContext = new QOpenGLContext();
    m_pBackgroundContext->setScreen(QGuiApplication::screens().at(0));
    m_pBackgroundContext->setShareContext(QOpenGLContext::globalShareContext());

    bool success = m_pBackgroundContext->create();
    Q_ASSERT(success);

    QSurfaceFormat format = m_pBackgroundContext->format();
    qDebug() << "OpenGL format acquired:" << format;

    QSurfaceFormat defaultFormat = QSurfaceFormat::defaultFormat();
    if ( format.majorVersion() != defaultFormat.majorVersion() ||
         format.minorVersion() != defaultFormat.minorVersion() )
    {
        qWarning().nospace() << "WARNING: Expected OpenGL format "
                             << defaultFormat.majorVersion() << "." << defaultFormat.minorVersion()
                             << ", got " << format.majorVersion() << "." << format.minorVersion()
                             << ". Some functionality may not work as intended.";
    }
}

void ResourceManager::setUpOpenGLResources()
{
    setUpShaders();
    setUpBuiltInTextures();

    m_pNumericFont = new SimpleNumericFont();
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

    PerVertexColorShader* pvcSh = new PerVertexColorShader();
    pvcSh->construct();
    m_Shaders.append(pvcSh);

    BasicLitTextureShader* bltSh = new BasicLitTextureShader();
    bltSh->construct();
    m_Shaders.append(bltSh);

    SelectionMaskShader* selmSh = new SelectionMaskShader();
    selmSh->construct();
    m_Shaders.append(selmSh);
}

void ResourceManager::setUpBuiltInTextures()
{
    // Default texture - returned if any other texture is not found.
    m_pDefaultTexture = new QOpenGLTexture(QImage(":/textures/error.png").mirrored());
    m_pDefaultTexture->setMinificationFilter(MIPMAP(QOpenGLTexture::NearestMipMapNearest));
    m_pDefaultTexture->setMagnificationFilter(MIPMAP(QOpenGLTexture::NearestMipMapNearest));
    m_pDefaultTexture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    m_pDefaultTexture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);

    // Other random ones.
    addBuiltInTexture(":/textures/test.png", MIPMAP(QOpenGLTexture::LinearMipMapLinear),
                      MIPMAP(QOpenGLTexture::LinearMipMapLinear));
    addBuiltInTexture(":/textures/uvsample.png", MIPMAP(QOpenGLTexture::NearestMipMapNearest),
                      MIPMAP(QOpenGLTexture::NearestMipMapNearest));
    addBuiltInTexture(":/textures/debug_crosshair.png", MIPMAP(QOpenGLTexture::NearestMipMapNearest),
                      MIPMAP(QOpenGLTexture::NearestMipMapNearest));
    addBuiltInTexture(":/textures/debug_translucent.png", MIPMAP(QOpenGLTexture::LinearMipMapLinear),
                      MIPMAP(QOpenGLTexture::LinearMipMapLinear));
    addBuiltInTexture(":/textures/dev/dev_purple001.png", MIPMAP(QOpenGLTexture::LinearMipMapLinear),
                      MIPMAP(QOpenGLTexture::LinearMipMapLinear));

    addBuiltInTexture(":/textures/ui/square-handle.png", QOpenGLTexture::Nearest,
                       QOpenGLTexture::Nearest);
    addBuiltInTexture(":/textures/ui/circle-handle.png", QOpenGLTexture::Nearest,
                       QOpenGLTexture::Nearest);
}

void ResourceManager::addBuiltInTexture(const QString &path, QOpenGLTexture::Filter min, QOpenGLTexture::Filter max,
                                        QOpenGLTexture::WrapMode s, QOpenGLTexture::WrapMode t)
{
    QOpenGLTexture* tex = new QOpenGLTexture(QImage(path).mirrored());
    tex->setMinificationFilter(min);
    tex->setMagnificationFilter(max);
    tex->setWrapMode(QOpenGLTexture::DirectionS, s);
    tex->setWrapMode(QOpenGLTexture::DirectionT, t);
    m_Textures.insert(path.mid(1).remove(".png"), tex);
}

void ResourceManager::makeCurrent()
{
    bool success = m_pBackgroundContext->makeCurrent(m_pSurface);
    Q_ASSERT(success);
}

void ResourceManager::doneCurrent()
{
    m_pBackgroundContext->doneCurrent();
}

ResourceManager::~ResourceManager()
{
    makeCurrent();

    delete m_pNumericFont;
    m_pNumericFont = nullptr;

    delete m_pFramebuffer;
    m_pFramebuffer = nullptr;

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
    Q_ASSERT(QOpenGLContext::currentContext());
    return QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();
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

    return nullptr;
}

QOpenGLTexture* ResourceManager::texture(const QString &path) const
{
    QOpenGLTexture* tex = m_Textures.value(path, nullptr);
    return tex ? tex : m_pDefaultTexture;
}

int ResourceManager::shaderIndex(const QString &name) const
{
    for ( int i = 0; i < m_Shaders.count(); i++ )
    {
        if ( m_Shaders.at(i)->objectName() == name )
            return i;
    }

    return -1;
}

SimpleNumericFont* ResourceManager::numericFont() const
{
    return m_pNumericFont;
}

QOpenGLFramebufferObject* ResourceManager::frameBuffer(const QSize &size)
{
    if ( m_pFramebuffer )
    {
        if ( m_pFramebuffer->width() == size.width() && m_pFramebuffer->height() == size.height() )
            return m_pFramebuffer;
        else
            delete m_pFramebuffer;
    }

    m_pFramebuffer = new QOpenGLFramebufferObject(size);
    return m_pFramebuffer;
}
