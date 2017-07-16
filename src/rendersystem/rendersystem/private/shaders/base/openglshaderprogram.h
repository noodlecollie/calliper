#ifndef OPENGLSHADERPROGRAM_H
#define OPENGLSHADERPROGRAM_H

#include <QOpenGLShaderProgram>

#include "rendersystem/private/shaders/common/vertexformat.h"

class OpenGLShaderProgram : public QOpenGLShaderProgram
{
public:
    // Subclasses should have a constructor which takes a quint16 ID and a default parent argument.
    OpenGLShaderProgram(const QString &name, QObject* parent = 0);
    virtual ~OpenGLShaderProgram();

    virtual void construct() = 0;
    virtual bool link() override;

    virtual VertexFormat vertexFormat() const = 0;

    // Maximum number of items supported in a batch.
    // Return 1 if the shader doesn't support batching.
    virtual int maxBatchedItems() const = 0;

    void setGlobalUniformBlockBinding();
    void setLocalUniformBlockBinding();

    // Only valid if the relevant function from above has been called.
    bool hasGlobalUniformBlockBinding() const;
    bool hasLocalUniformBlockBinding() const;

    bool bindWithUniforms();

    bool addShaderWithCommonHeaders(QOpenGLShader::ShaderType type, const QByteArray &shader);
    bool addShaderWithCommonHeaders(QOpenGLShader::ShaderType type, const char* shader);
    bool addShaderFileWithCommonHeaders(QOpenGLShader::ShaderType type, const QString &filePath);

private:
    void cacheCommonHeaders();

    GLuint m_iGlobalShaderBlockIndex;
    GLuint m_iLocalShaderBlockIndex;
};

#endif // OPENGLSHADERPROGRAM_H
