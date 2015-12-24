#include "minimumtexturedshader.h"
#include <QOpenGLFunctions_4_1_Core>
#include "resourcemanager.h"

MinimumTexturedShader::MinimumTexturedShader() : ShaderProgram("MinimumTexturedShader")
{
    m_iPositionLocation = 0;
    m_iUVLocation = 0;
}

void MinimumTexturedShader::construct()
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    bool success = create();
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::VertexShader, ":/shaders/minimum_textured.vert");
    Q_ASSERT(success);

    success = compileFile(ShaderProgram::FragmentShader, ":/shaders/minimum_textured.frag");
    Q_ASSERT(success);

    link();

    m_iPositionLocation = f->glGetAttribLocation(handle(), "position");
    m_iUVLocation = f->glGetAttribLocation(handle(), "uv");
}

void MinimumTexturedShader::apply() const
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

    f->glEnableVertexAttribArray(m_iUVLocation);
    f->glVertexAttribPointer(
       m_iUVLocation,
       2,                   // size
       GL_FLOAT,            // type
       GL_FALSE,            // normalized?
       8*sizeof(float),     // stride
       (void*)(6*sizeof(float))             // array buffer offset
    );
}

void MinimumTexturedShader::release() const
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glDisableVertexAttribArray(m_iPositionLocation);
    f->glDisableVertexAttribArray(m_iUVLocation);
}
