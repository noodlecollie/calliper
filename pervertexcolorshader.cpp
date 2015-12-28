#include "pervertexcolorshader.h"
#include <QOpenGLFunctions_4_1_Core>
#include "callipermath.h"
#include "resourcemanager.h"

PerVertexColorShader::PerVertexColorShader() : ShaderProgram(PerVertexColorShader::staticName())
{
}

QString PerVertexColorShader::staticName()
{
    return QString("PerVertexColorShader");
}

void PerVertexColorShader::construct()
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    bool success = create();
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::VertexShader, ":/shaders/pervertexcolor.vert");
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::FragmentShader, ":/shaders/pervertexcolor.frag");
    Q_ASSERT(success);

    link();

    m_iAttributeLocations[Position] = f->glGetAttribLocation(handle(), "vPositionModelSpace");
    m_iAttributeLocations[Color] = f->glGetAttribLocation(handle(), "vColor");
    m_iAttributeLocations[ModelToWorldMatrix] = f->glGetUniformLocation(handle(), "modelToWorld");
    m_iAttributeLocations[WorldToCameraMatrix] = f->glGetUniformLocation(handle(), "worldToCamera");
    m_iAttributeLocations[CoordinateTransformMatrix] = f->glGetUniformLocation(handle(), "hammerToOpenGL");
    m_iAttributeLocations[CameraProjectionMatrix] = f->glGetUniformLocation(handle(), "projection");
}

void PerVertexColorShader::apply() const
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glUseProgram(handle());

    f->glEnableVertexAttribArray(m_iAttributeLocations[Position]);
    f->glEnableVertexAttribArray(m_iAttributeLocations[Color]);
}

void PerVertexColorShader::release() const
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glDisableVertexAttribArray(m_iAttributeLocations[Position]);
    f->glDisableVertexAttribArray(m_iAttributeLocations[Color]);
}
