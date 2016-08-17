#include "selectionmaskshader.h"
#include <QOpenGLFunctions_4_1_Core>
#include "resourcemanager.h"

SelectionMaskShader::SelectionMaskShader() : ShaderProgram(SelectionMaskShader::staticName())
{

}

QString SelectionMaskShader::staticName()
{
    return QString("SelectionMaskShader");
}

void SelectionMaskShader::construct()
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    bool success = create();
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::VertexShader, ":/shaders/selectionmask.vert");
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::FragmentShader, ":/shaders/selectionmask.frag");
    Q_ASSERT(success);

    link();

    m_iAttributeLocations[Position] = f->glGetAttribLocation(handle(), "vPositionModelSpace");
    m_iAttributeLocations[Color] = f->glGetAttribLocation(handle(), "vColor");
    m_iAttributeLocations[ModelToWorldMatrix] = f->glGetUniformLocation(handle(), "modelToWorld");
    m_iAttributeLocations[WorldToCameraMatrix] = f->glGetUniformLocation(handle(), "worldToCamera");
    m_iAttributeLocations[CoordinateTransformMatrix] = f->glGetUniformLocation(handle(), "hammerToOpenGL");
    m_iAttributeLocations[CameraProjectionMatrix] = f->glGetUniformLocation(handle(), "projection");
    m_iAttributeLocations[CounterScaleUniform] = f->glGetUniformLocation(handle(), "counterScale");
    m_iAttributeLocations[ColorUniform] = f->glGetUniformLocation(handle(), "inCol");
}

void SelectionMaskShader::apply() const
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glUseProgram(handle());

    f->glEnableVertexAttribArray(m_iAttributeLocations[Position]);
    f->glEnableVertexAttribArray(m_iAttributeLocations[Color]);
}

void SelectionMaskShader::release() const
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glDisableVertexAttribArray(m_iAttributeLocations[Position]);
    f->glDisableVertexAttribArray(m_iAttributeLocations[Color]);
}
