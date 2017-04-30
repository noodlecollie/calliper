#ifndef OPENGLSHADERPROGRAM_H
#define OPENGLSHADERPROGRAM_H

#include <QOpenGLShaderProgram>
#include "ishaderspec.h"

class OpenGLShaderProgram : public QOpenGLShaderProgram, public IShaderSpec
{
public:
    // Subclasses should have a constructor which takes a quint16 ID and a default parent argument.
    OpenGLShaderProgram(const QString &name, QObject* parent = 0);
    virtual ~OpenGLShaderProgram();

    virtual void construct() = 0;
    virtual bool link() override;

    // Should be bound before calling, and released after!
    virtual void setGlobalUniformBlockBinding();
    void setLocalUniformBlockBinding();
    virtual bool hasLocalUniformBlockBinding() const;

    void enableAttributeArrays();
    void disableAttributeArrays();

    bool addShaderWithCommonHeaders(QOpenGLShader::ShaderType type, const QByteArray &shader);
    bool addShaderWithCommonHeaders(QOpenGLShader::ShaderType type, const char* shader);
    bool addShaderFileWithCommonHeaders(QOpenGLShader::ShaderType type, const QString &filePath);

private:
    void cacheCommonHeaders();
    GLuint m_iGlobalShaderBlockIndex;
    GLuint m_iLocalShaderBlockIndex;
};

#endif // OPENGLSHADERPROGRAM_H
