#include "temporaryrender.h"
#include <QtDebug>
#include "resourcemanager.h"
#include "shaderprogram.h"
#include "openglrenderer.h"

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

void temporarySetup(QOpenGLContext *context, QOpenGLFunctions_4_1_Core *f)
{
    resourceManager()->setLiveContext(context);

    // =============================================
    // General OpenGL admin
    // =============================================
    f->glGenVertexArrays(1, &VertexArrayID);
    f->glBindVertexArray(VertexArrayID);

    // =============================================
    // Set up geometry
    // =============================================
    f->glGenBuffers(1, &vertexbuffer);
    f->glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    f->glBufferData(GL_ARRAY_BUFFER, VERTICES_SIZE, vertices, GL_STATIC_DRAW);

    f->glGenBuffers(1, &indexbuffer);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES_SIZE, indices, GL_STATIC_DRAW);

    // =============================================
    // Set up shaders
    // =============================================

    /*shader = new ShaderProgram("Default");
    shader->create();
    shader->compileSource(ShaderProgram::VertexShader, vertexShader);
    shader->compileSource(ShaderProgram::FragmentShader, fragmentShader);
    shader->link();
    shader->bind(true);

    locColour = f->glGetUniformLocation(shader->handle(), "inCol");*/

    renderer()->setGlobalColor(QColor(255,0,0));

    resourceManager()->setLiveContext(NULL);
}

void temporaryRender(QOpenGLContext *context, QOpenGLFunctions_4_1_Core *f)
{
    resourceManager()->setLiveContext(context);

    // =============================================
    // Specify vertex format
    // =============================================
    f->glEnableVertexAttribArray(0);
    f->glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    f->glVertexAttribPointer(
       0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,   // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,        // stride
       (void*)0  // array buffer offset
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);

    resourceManager()->minimumShader()->apply();

    // =============================================
    // Draw vertices
    // =============================================
    //f->glDrawArrays(GL_TRIANGLES, 0, 6);
    f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    f->glDisableVertexAttribArray(0);

    resourceManager()->setLiveContext(NULL);
}
