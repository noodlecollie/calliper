#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QByteArray>
#include <QMatrix4x4>
#include <QColor>

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

    enum Attribute
    {
        Position = 0,
        Normal,
        Color,
        UV,

        ColorUniform,
        DirectionalLightUniform,
        FogColorUniform,
        FogBeginUniform,
        FogEndUniform,
        CounterScaleUniform,

        ModelToWorldMatrix,
        WorldToCameraMatrix,
        CoordinateTransformMatrix,
        CameraProjectionMatrix,

        AttributeCount
    };

    bool compileSource(ShaderType type, const char* source);
    bool compileFile(ShaderType type, const QString &filename);
    bool create();
    void link();
    void bind(bool b);

    virtual void construct() = 0;
    virtual void apply() const = 0;
    virtual void release() const = 0;

    void setAttributeFormat(Attribute att, int components, int strideBytes, int offsetBytes);
    void setUniformMatrix4(Attribute att, const QMatrix4x4 &mat);
    void setUniformVector3(Attribute att, const QVector3D &vec);
    void setUniformColor4(Attribute att, const QColor &col);
    void setUniformFloat(Attribute att, float val);

    GLuint handle() const;

signals:

public slots:

protected:
    bool compile(GLuint* shaderHandle, GLuint shaderType, const char* source);

    GLuint  m_iShaderProgram;
    GLuint  m_iVertexShader;
    GLuint  m_iFragmentShader;

    GLuint  m_iAttributeLocations[AttributeCount];

    QByteArray  m_LogBuffer;
};

#endif // SHADERPROGRAM_H
