#include "temporaryrender.h"
#include <QtDebug>
#include "resourcemanager.h"
#include "shaderprogram.h"

const float vertices[] = {
    -0.8f, -0.8f, 0.0f,
    0.8f, -0.8f, 0.0f,
    0.8f, 0.8f, 0.0f,
    -0.8f, -0.8f, 0.0f,
    0.8f, 0.8f, 0.0f,
    -0.8f, 0.8f, 0.0f,
};

#define VERTICES_SIZE (sizeof(float)*3*6)

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

    // =============================================
    // Set up shaders
    // =============================================
    /*VertexShaderID = f->glCreateShader(GL_VERTEX_SHADER);
    FragmentShaderID = f->glCreateShader(GL_FRAGMENT_SHADER);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    f->glShaderSource(VertexShaderID, 1, &vertexShader , NULL);
    f->glCompileShader(VertexShaderID);

    f->glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    f->glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 )
    {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength);
        f->glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        qDebug() << "Vertex shader compilation messages:" << &VertexShaderErrorMessage[0];
    }

    f->glShaderSource(FragmentShaderID, 1, &fragmentShader , NULL);
    f->glCompileShader(FragmentShaderID);

    f->glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    f->glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 )
    {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
        f->glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        qDebug() << "Fragment shader compilation messages:" << &FragmentShaderErrorMessage[0];
    }

    ProgramID = f->glCreateProgram();
    f->glAttachShader(ProgramID, VertexShaderID);
    f->glAttachShader(ProgramID, FragmentShaderID);
    f->glLinkProgram(ProgramID);

    f->glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    f->glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 )
    {
        std::vector<char> ProgramErrorMessage(InfoLogLength);
        f->glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        qDebug() << "Shader program linking messages:" <<  &ProgramErrorMessage[0];
    }

    f->glDeleteShader(VertexShaderID);
    f->glDeleteShader(FragmentShaderID);
    f->glUseProgram(ProgramID);*/

    shader = new ShaderProgram("Default");
    shader->create();
    shader->compileSource(ShaderProgram::VertexShader, vertexShader);
    shader->compileSource(ShaderProgram::FragmentShader, fragmentShader);
    shader->link();
    shader->bind(true);

    locColour = f->glGetUniformLocation(shader->handle(), "inCol");

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

    f->glUniform3f(locColour, mainColour[0], mainColour[1], mainColour[2]);

    // =============================================
    // Draw vertices
    // =============================================
    f->glDrawArrays(GL_TRIANGLES, 0, 6);

    f->glDisableVertexAttribArray(0);

    resourceManager()->setLiveContext(NULL);
}
