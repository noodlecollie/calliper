#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QByteArray>

class ShaderProgram : public QObject
{
    Q_OBJECT
public:
    explicit ShaderProgram(const QString &name);
    ~ShaderProgram();

    enum ShaderType
    {
        VertexShader = 0,
        FragmentShader
    };

    bool compileSource(ShaderType type, const char* source);
    bool compileFile(ShaderType type, const QString &filename);
    bool create();
    void link();
    void bind(bool b);

signals:

public slots:

private:
    bool compile(GLuint* shaderHandle, GLuint shaderType, const char* source);

    GLuint  m_iShaderProgram;
    GLuint  m_iVertexShader;
    GLuint  m_iFragmentShader;

    QByteArray  m_LogBuffer;
};

#endif // SHADERPROGRAM_H
