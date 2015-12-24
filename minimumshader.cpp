#include "minimumshader.h"
#include <QtDebug>
#include "resourcemanager.h"
#include "openglrenderer.h"

MinimumShader::MinimumShader() : ShaderProgram("MinimumShader")
{
    m_iColourLocation = 0;
}

void MinimumShader::construct()
{
    bool success = create();
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::VertexShader, ":/shaders/minimum.vert");
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::FragmentShader, ":/shaders/minimum.frag");
    Q_ASSERT(success);

    link();
    m_iColourLocation = resourceManager()->functions()->glGetUniformLocation(handle(), "inCol");
    m_iPositionLocation = resourceManager()->functions()->glGetAttribLocation(handle(), "position");
}

void MinimumShader::apply() const
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glUseProgram(handle());

    f->glEnableVertexAttribArray(m_iPositionLocation);
    f->glVertexAttribPointer(
       m_iPositionLocation,
       3,                   // size
       GL_FLOAT,            // type
       GL_FALSE,            // normalized?
       8*sizeof(float),     // stride
       (void*)0             // array buffer offset
    );

    QColor col = renderer()->globalColor();
    f->glUniform4f(m_iColourLocation, col.redF(), col.greenF(), col.blueF(), col.alphaF());
}

void MinimumShader::release() const
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glDisableVertexAttribArray(m_iPositionLocation);
}
