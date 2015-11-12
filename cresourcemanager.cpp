#include "cresourcemanager.h"
#include <QOffscreenSurface>
#include <QGuiApplication>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QFile>

bool attemptCompile(QOpenGLShaderProgram* program, QOpenGLShader::ShaderType type, const QString &path)
{
    if ( !program->addShaderFromSourceFile(type, path) )
    {
        qWarning().nospace() << "Compilation failed for " << program->objectName() << " with shader file " << path << ": " << program->log();
        return false;
    }

    return true;
}

bool attemptLink(QOpenGLShaderProgram* program)
{
    if ( !program->link() )
    {
        qWarning().nospace() << "Linking failed for shader " << program->objectName() << ": " << program->log();
        return false;
    }

    return true;
}

CResourceManager::CResourceManager(QObject *parent) : QObject(parent)
{
    m_pSurface = new QOffscreenSurface();
    m_pSurface->setFormat(QSurfaceFormat::defaultFormat());
    m_pSurface->create();
    
    m_pBackgroundContext = new QOpenGLContext(this);
    m_pBackgroundContext->setFormat(QSurfaceFormat::defaultFormat());
    m_pBackgroundContext->setScreen(QGuiApplication::screens().at(0));
    m_pBackgroundContext->setShareContext(QOpenGLContext::globalShareContext());

    bool success = m_pBackgroundContext->create();
    Q_ASSERT(success);
    
    // Load our default resources before anything else.
    loadFailsafes();
}

CResourceManager::~CResourceManager()
{
    m_pBackgroundContext->makeCurrent(m_pSurface);
    
    QList<QOpenGLTexture*> textures = m_TextureTable.values();
    qDeleteAll(textures);
    
    QList<QOpenGLShaderProgram*> shaders = m_ShaderTable.values();
    qDeleteAll(shaders);
    
    m_pBackgroundContext->doneCurrent();
    delete m_pBackgroundContext;
    
    m_pSurface->destroy();
    delete m_pSurface;
}

QOffscreenSurface* CResourceManager::generalSurface() const
{
    return m_pSurface;
}

QOpenGLContext* CResourceManager::backgroundContext() const
{
    return m_pBackgroundContext;
}

bool CResourceManager::addShader(const QString &name, const QString &vertFile, const QString &fragFile, QString *log)
{
    if ( name.isEmpty() ) return false;
    
    QOpenGLShaderProgram* p = new QOpenGLShaderProgram(m_pBackgroundContext);
    p->setObjectName(name);
    
    switch(0)
    {
    case 0:
    default:
        if ( !attemptCompile(p, QOpenGLShader::Vertex, vertFile) ) break;
        if ( !attemptCompile(p, QOpenGLShader::Fragment, fragFile) ) break;
        if ( !attemptLink(p) ) break;
        
        // Everything succeeded.
        m_ShaderTable.insert(name, p);
        return true;
    }
    
    // Something failed.
    if ( log ) *log = p->log();
    delete p;
    return false;
}

QOpenGLShaderProgram* CResourceManager::shader(const QString &name) const
{
    return m_ShaderTable.value(name, NULL);
}

bool CResourceManager::shaderExists(const QString &name) const
{
    return m_ShaderTable.contains(name);
}

bool CResourceManager::loadTexture(const QUrl &uri, QOpenGLTexture::Filter min, QOpenGLTexture::Filter max)
{
    if ( uri.isEmpty() ) return false;
    
    QOpenGLTexture* texture = loadRawTexture(uri, min, max);
    if ( !texture ) return false;
    
    // Delete any existing texture under this URI.
    QOpenGLTexture* existing = m_TextureTable.value(uri, NULL);
    if ( existing )
    {
        existing->destroy();
        delete existing;
    }
    
    m_TextureTable.insert(uri, texture);
    
    return true;
}

QOpenGLTexture* CResourceManager::loadRawTexture(const QUrl &uri, QOpenGLTexture::Filter min, QOpenGLTexture::Filter max)
{
    QString path;
    if ( uri.scheme() == "qrc" )
    {
        // TODO: Is this the best way? toLocalFile() doesn't work with qrc scheme.
        QString s = uri.toString();
        int index = s.indexOf(':');
        path = index >= 0 ? s.mid(index) : s;
    }
    else
    {
        path = uri.toLocalFile();
    }
    if ( !QFile::exists(path) ) return NULL;
    
    QOpenGLTexture* texture = new QOpenGLTexture(QImage(path).mirrored());
    texture->setMinificationFilter(min);
    texture->setMagnificationFilter(max);
    
    if ( !texture->create() )
    {
        delete texture;
        return NULL;
    }
    
    return texture;
}

QOpenGLTexture* CResourceManager::texture(const QUrl &uri) const
{
    return m_TextureTable.value(uri, NULL);
}

bool CResourceManager::textureExists(const QUrl &uri) const
{
    return m_TextureTable.contains(uri);
}

QOpenGLTexture* CResourceManager::fetchTexture(const QUrl &uri, QOpenGLTexture::Filter min, QOpenGLTexture::Filter max)
{
    if ( uri.isEmpty() ) return NULL;
    
    QOpenGLTexture* tex = texture(uri);
    if ( tex ) return tex;
    
    if ( !loadTexture(uri, min, max) ) return NULL;
    return texture(uri);
}

void CResourceManager::makeCurrent()
{
    m_pBackgroundContext->makeCurrent(m_pSurface);
}

void CResourceManager::doneCurrent()
{
    m_pBackgroundContext->doneCurrent();
}

void CResourceManager::loadFailsafes()
{
    makeCurrent();
    bool success = false;
    
    success = addShader(SHADER_NAME_FALLBACK, SHADER_VERT_FALLBACK, SHADER_FRAG_FALLBACK);
    Q_ASSERT(success);
    
    success = loadTexture(QUrl(TEXTURE_URI_FALLBACK));
    Q_ASSERT(success);
}

QOpenGLShaderProgram* CResourceManager::fallbackShader() const
{
    return shader(SHADER_NAME_FALLBACK);
}

QOpenGLTexture* CResourceManager::fallbackTexture() const
{
    return texture(QUrl(TEXTURE_URI_FALLBACK));
}
