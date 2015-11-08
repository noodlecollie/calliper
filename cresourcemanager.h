#ifndef CRESOURCEMANAGER_H
#define CRESOURCEMANAGER_H

#include <QObject>
#include <QHash>

class QOffscreenSurface;
class QOpenGLContext;
class QOpenGLShaderProgram;

class CResourceManager : public QObject
{
    Q_OBJECT
public:
    explicit CResourceManager(QObject *parent = 0);
    ~CResourceManager();
    
    // OpenGL-related
    QOffscreenSurface* generalSurface() const;
    QOpenGLContext* backgroundContext() const;
    
    // Shaders
    bool addShader(const QString &name, const QString &vertFile, const QString &fragFile, QString *log = NULL);
    QOpenGLShaderProgram* shader(const QString &name) const;
    
signals:
    
public slots:
    
private:
    typedef QHash<QString, QOpenGLShaderProgram*> ShaderTable;
    
    QOffscreenSurface*  m_pSurface;
    QOpenGLContext*     m_pBackgroundContext;
    ShaderTable         m_ShaderTable;
};

#endif // CRESOURCEMANAGER_H
