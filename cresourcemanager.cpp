#include "cresourcemanager.h"
#include <QOffscreenSurface>
#include <QGuiApplication>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>

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
}

CResourceManager::~CResourceManager()
{
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
