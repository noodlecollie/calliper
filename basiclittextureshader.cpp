#include "basiclittextureshader.h"
#include <QOpenGLFunctions_4_1_Core>
#include "resourcemanager.h"

BasicLitTextureShader::BasicLitTextureShader() : ShaderProgram(BasicLitTextureShader::staticName())
{

}

QString BasicLitTextureShader::staticName()
{
    return QString("BasicLitTextureShader");
}

void BasicLitTextureShader::construct()
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    bool success = create();
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::VertexShader, ":/shaders/basiclit.vert");
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::FragmentShader, ":/shaders/basiclit.frag");
    Q_ASSERT(success);

    link();

    m_iAttributeLocations[Position] = f->glGetAttribLocation(handle(), "vPositionModelSpace");
    m_iAttributeLocations[Normal] = f->glGetAttribLocation(handle(), "vNormal");
    m_iAttributeLocations[UV] = f->glGetAttribLocation(handle(), "vUV");
    m_iAttributeLocations[ColorUniform] = f->glGetUniformLocation(handle(), "fColor");
    m_iAttributeLocations[FogColorUniform] = f->glGetUniformLocation(handle(), "fFogColor");
    m_iAttributeLocations[FogBeginUniform] = f->glGetUniformLocation(handle(), "fFogBegin");
    m_iAttributeLocations[FogEndUniform] = f->glGetUniformLocation(handle(), "fFogEnd");
    m_iAttributeLocations[DirectionalLightUniform] = f->glGetUniformLocation(handle(), "directionalLight");
    m_iAttributeLocations[ModelToWorldMatrix] = f->glGetUniformLocation(handle(), "modelToWorld");
    m_iAttributeLocations[WorldToCameraMatrix] = f->glGetUniformLocation(handle(), "worldToCamera");
    m_iAttributeLocations[CoordinateTransformMatrix] = f->glGetUniformLocation(handle(), "hammerToOpenGL");
    m_iAttributeLocations[CameraProjectionMatrix] = f->glGetUniformLocation(handle(), "projection");
}

void BasicLitTextureShader::apply() const
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glUseProgram(handle());

    f->glEnableVertexAttribArray(m_iAttributeLocations[Position]);
    f->glEnableVertexAttribArray(m_iAttributeLocations[Normal]);
    f->glEnableVertexAttribArray(m_iAttributeLocations[UV]);
}

void BasicLitTextureShader::release() const
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glDisableVertexAttribArray(m_iAttributeLocations[Position]);
    f->glDisableVertexAttribArray(m_iAttributeLocations[Normal]);
    f->glDisableVertexAttribArray(m_iAttributeLocations[UV]);
}

