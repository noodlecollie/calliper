#ifndef CRESOURCEMANAGER_H
#define CRESOURCEMANAGER_H

#include <QObject>
#include <QHash>
#include <QUrl>
#include <QOpenGLTexture>

class QOffscreenSurface;
class QOpenGLContext;
class QOpenGLShaderProgram;

#define SHADER_NAME_FALLBACK    "SHADER_FALLBACK"
#define SHADER_VERT_FALLBACK    ":/shaders/plain.vert"
#define SHADER_FRAG_FALLBACK    ":/shaders/solidcolor.frag"
#define TEXTURE_URI_FALLBACK    "qrc:/TEX_ERROR"

class CResourceManager : public QObject
{
    Q_OBJECT
public:
    explicit CResourceManager(QObject *parent = 0);
    ~CResourceManager();
    
    // OpenGL-related
    QOffscreenSurface* generalSurface() const;
    QOpenGLContext* backgroundContext() const;
    void makeCurrent();
    void doneCurrent();
    
    // Shaders
    QOpenGLShaderProgram* fallbackShader() const;
    bool addShader(const QString &name, const QString &vertFile, const QString &fragFile, QString *log = NULL);
    QOpenGLShaderProgram* shader(const QString &name) const;
    bool shaderExists(const QString &name) const;
    
    // Textures
    QOpenGLTexture* fallbackTexture() const;
    bool loadTexture(const QUrl &uri, QOpenGLTexture::Filter min = QOpenGLTexture::Nearest, QOpenGLTexture::Filter max = QOpenGLTexture::Nearest);
    QOpenGLTexture* texture(const QUrl &uri) const;
    bool textureExists(const QUrl &uri) const;
    QOpenGLTexture* fetchTexture(const QUrl &uri, QOpenGLTexture::Filter min = QOpenGLTexture::Nearest, QOpenGLTexture::Filter max = QOpenGLTexture::Nearest);
    
signals:
    
public slots:
    
private:
    typedef QHash<QString, QOpenGLShaderProgram*> ShaderTable;
    typedef QHash<QUrl, QOpenGLTexture*> TextureTable;
    
    void loadFailsafes();
    QOpenGLTexture* loadRawTexture(const QUrl &uri, QOpenGLTexture::Filter min = QOpenGLTexture::Nearest, QOpenGLTexture::Filter max = QOpenGLTexture::Nearest);
    
    QOffscreenSurface*  m_pSurface;
    QOpenGLContext*     m_pBackgroundContext;
    ShaderTable         m_ShaderTable;
    TextureTable        m_TextureTable;
};

#endif // CRESOURCEMANAGER_H
