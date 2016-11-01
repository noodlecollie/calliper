#include "unlittextureshader.h"
#include <QOpenGLFunctions_4_1_Core>
#include "callipermath.h"
#include "openglrenderer.h"
#include "resourcemanager.h"
#include <QtDebug>

UnlitTextureShader::UnlitTextureShader() : ShaderProgram(UnlitTextureShader::staticName())
{
}

QString UnlitTextureShader::staticName()
{
    return QString("UnlitTextureShader");
}

void UnlitTextureShader::construct()
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    bool success = create();
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::VertexShader, ":/shaders/unlit.vert");
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::FragmentShader, ":/shaders/unlit.frag");
    Q_ASSERT(success);

    link();

    m_iAttributeLocations[Position] = f->glGetAttribLocation(handle(), "vPositionModelSpace");
    m_iAttributeLocations[UV] = f->glGetAttribLocation(handle(), "vUV");
    m_iAttributeLocations[ModelToWorldMatrix] = f->glGetUniformLocation(handle(), "modelToWorld");
    m_iAttributeLocations[WorldToCameraMatrix] = f->glGetUniformLocation(handle(), "worldToCamera");
    m_iAttributeLocations[CoordinateTransformMatrix] = f->glGetUniformLocation(handle(), "hammerToOpenGL");
    m_iAttributeLocations[CameraProjectionMatrix] = f->glGetUniformLocation(handle(), "projection");
    m_iAttributeLocations[CounterScaleUniform] = f->glGetUniformLocation(handle(), "counterScale");
}

void UnlitTextureShader::apply() const
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glUseProgram(handle());

    f->glEnableVertexAttribArray(m_iAttributeLocations[Position]);
    f->glEnableVertexAttribArray(m_iAttributeLocations[UV]);
}

void UnlitTextureShader::release() const
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glDisableVertexAttribArray(m_iAttributeLocations[Position]);
    f->glDisableVertexAttribArray(m_iAttributeLocations[UV]);
}
