#include "demoglwindow.h"
#include <QScreen>

static const char *vertexShaderSource =
        "attribute highp vec3 vPosition;"
        "attribute highp vec3 vNormal;"
        "attribute lowp vec4 vColor;"
        "varying lowp vec4 fVColor;"
        "varying highp vec3 fNormal;"
        "uniform highp mat4 modelToWorld;"
        "uniform highp mat4 worldToCamera;"
        "uniform highp mat4 hammerToOpenGL;"
        "uniform highp mat4 projection;"
        "void main(){"
            "gl_Position = projection * hammerToOpenGL * worldToCamera * modelToWorld * vec4(vPosition, 1);"
            "fVColor = vColor;"
            "fNormal = normalize((modelToWorld * vec4(vNormal, 0)).xyz);"
        "}"
    ;

static const char *fragmentShaderSource =
        "varying lowp vec4 fVColor;"
        "varying highp vec3 fNormal;"
        "void main(){"
            "gl_FragColor = fVColor;"
        "}"
    ;

DemoGLWindow::DemoGLWindow()
{
}

DemoGLWindow::~DemoGLWindow()
{
}

void DemoGLWindow::initializeGL()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("vPosition");
    m_colAttr = m_program->attributeLocation("vColor");
    m_nrmAttr = m_program->attributeLocation("vNormal");

    m_modelWorldUniform = m_program->uniformLocation("modelToWorld");
    m_worldCameraUniform = m_program->uniformLocation("worldToCamera");
    m_coordTransformUniform = m_program->uniformLocation("hammerToOpenGL");
    m_projectionUniform = m_program->uniformLocation("projection");

    GLfloat atts[] = {
        0.0f, 0.707f, 0,    // 9 floats for position
        -0.5f, -0.5f, 0,
        0.5f, -0.5f, 0,

        1.0f, 0.0f, 0.0f, 1,    // 12 foats for colour
        0.0f, 1.0f, 0.0f, 1,
        0.0f, 0.0f, 1.0f, 1,

        0,0,1,  // 9 floats for normal
        0,0,1,
        0,0,1,
    };  // 30 in total

    m_pVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_pVertexBuffer->create();
    m_pVertexBuffer->bind();
    m_pVertexBuffer->allocate(atts, 30*sizeof(GLfloat));

    GLushort indices[] =
    {
        0,1,2
    };

    m_pIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_pIndexBuffer->create();
    m_pIndexBuffer->bind();
    m_pIndexBuffer->allocate(indices, 3*sizeof(GLushort));
}

void DemoGLWindow::resizeGL(int w, int h)
{
    QOpenGLWindow::resizeGL(w,h);
}

void DemoGLWindow::paintGL()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 modelToWorld;
    modelToWorld.translate(0, 0, -2);

    QMatrix4x4 worldToCamera;
    QMatrix4x4 coordTransform;
    QMatrix4x4 projection;
    projection.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);

    m_program->setUniformValue(m_modelWorldUniform, modelToWorld);
    m_program->setUniformValue(m_worldCameraUniform, worldToCamera);
    m_program->setUniformValue(m_coordTransformUniform, coordTransform);
    m_program->setUniformValue(m_projectionUniform, projection);

    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(m_colAttr, 4, GL_FLOAT, GL_FALSE, 0, (void*)(9*sizeof(GLfloat)));
    glVertexAttribPointer(m_nrmAttr, 3, GL_FLOAT, GL_FALSE, 0, (void*)((9+12)*sizeof(GLfloat)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

//    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (void*)0);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(2);

    m_program->release();
}
