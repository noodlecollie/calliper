#include "temporaryrender.h"
#include <QtDebug>
#include "resourcemanager.h"
#include "shaderprogram.h"
#include "openglrenderer.h"
#include <QOpenGLBuffer>
#include "geometrydata.h"

const float vertices[] = {
    -0.8f, -0.8f, 0.0f,
    0.8f, -0.8f, 0.0f,
    0.8f, 0.8f, 0.0f,
    -0.8f, 0.8f, 0.0f,
};

const unsigned int indices[] = {
    0,1,2,
    0,2,3,
};

#define VERTICES_SIZE (sizeof(float)*3*4)
#define INDICES_SIZE (sizeof(unsigned int)*3*2)

const char* vertexShader =
        "#version 410 core\n"
        "in vec3 position;\n"
        "void main(){\n"
        "gl_Position = vec4(position, 1);\n"
        "}\n";

const char* fragmentShader =
        "#version 410 core\n"
        "uniform vec3 inCol;\n"
        "out vec4 color;\n"
        "void main(){\n"
        "color = vec4(inCol, 1); }\n";

const float mainColour[] = {
    1.0f, 0.4f, 0.6f,
};

GLuint VertexArrayID = 0;
GLuint vertexbuffer = 0;
GLuint indexbuffer = 0;
GLuint VertexShaderID = 0;
GLuint FragmentShaderID = 0;
GLuint ProgramID = 0;
GLuint locColour = 0;
ShaderProgram* shader = NULL;

QOpenGLBuffer* pVertexBuffer = NULL;
QOpenGLBuffer* pIndexBuffer = NULL;
GeometryData* geometry = NULL;

void temporarySetup(QOpenGLContext *context, QOpenGLFunctions_4_1_Core *f)
{
    resourceManager()->setLiveContext(context);

    // =============================================
    // General OpenGL admin
    // =============================================
//    f->glGenVertexArrays(1, &VertexArrayID);
//    f->glBindVertexArray(VertexArrayID);

    // =============================================
    // Set up geometry
    // =============================================

    geometry = new GeometryData();
    geometry->appendVertex(QVector3D(-0.8f, -0.8f, 0.0f), QVector3D(0,0,1), QVector2D(0,0));
    geometry->appendVertex(QVector3D(0.8f, -0.8f, 0.0f), QVector3D(0,0,1), QVector2D(1,0));
    geometry->appendVertex(QVector3D(0.8f, 0.8f, 0.0f), QVector3D(0,0,1), QVector2D(1,1));
    geometry->appendVertex(QVector3D(-0.8f, 0.8f, 0.0f), QVector3D(0,0,1), QVector2D(0,1));
    geometry->appendIndex(0);
    geometry->appendIndex(1);
    geometry->appendIndex(2);
    geometry->appendIndex(0);
    geometry->appendIndex(2);
    geometry->appendIndex(3);
    geometry->upload();

    // =============================================
    // Set up shaders
    // =============================================
    renderer()->setGlobalColor(QColor(255,0,0));

    resourceManager()->setLiveContext(NULL);
}

void temporaryRender(QOpenGLContext *context, QOpenGLFunctions_4_1_Core *f)
{
    resourceManager()->setLiveContext(context);

    // =============================================
    // Specify vertex format
    // =============================================
    geometry->bindVertices(true);
    geometry->bindIndices(true);

    resourceManager()->minimumShader()->apply();

    // =============================================
    // Draw vertices
    // =============================================
    geometry->draw();

    resourceManager()->minimumShader()->release();

    resourceManager()->setLiveContext(NULL);
}
