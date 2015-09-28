#include "cviewport.h"
#include <QtMath>

/*const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};*/

const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

#define VERTEX_DATA_SIZE (9 * sizeof(GLfloat))

const char* vertexShader =
        "#version 330 core\n"
        "in vec3 vertexPosition_modelspace;\n"
        "uniform mat4 MVP;\n"
        "void main(){\n"
        "vec4 v = vec4(vertexPosition_modelspace,1);\n"
        "gl_Position = MVP * v; }\n";

// Non-matrix
/*const char* vertexShader =
        "#version 330 core\n"
        "layout(location = 0) in vec3 vertexPosition_modelspace;\n"
        "void main(){\n"
        "gl_Position.xyz = vertexPosition_modelspace;\n"
        "gl_Position.w = 1.0; }\n";*/

const char* fragmentShader =
        "#version 330 core\n"
        "out vec3 color;\n"
        "void main(){\n"
        "color = vec3(1,0,0);}\n";

void ComputeFOVProjection(QMatrix4x4& result, float fov, float aspect, float nearDist, float farDist, bool leftHanded = true )
{
    //
    // General form of the Projection Matrix
    //
    // uh = Cot( fov/2 ) == 1/Tan(fov/2)
    // uw / uh = 1/aspect
    //
    //   uw         0       0       0
    //    0        uh       0       0
    //    0         0      f/(f-n)  1
    //    0         0    -fn/(f-n)  0
    //
    // Make result to be identity first

    // check for bad parameters to avoid divide by zero:
    // if found, assert and return an identity matrix.
    if ( fov <= 0 || aspect == 0 )
    {
        Q_ASSERT( fov > 0 && aspect != 0 );
        return;
    }

    float frustumDepth = farDist - nearDist;
    float oneOverDepth = 1 / frustumDepth;

    result(1,1) = 1 / qTan(0.5f * fov);
    result(0,0) = (leftHanded ? 1 : -1 ) * result(1,1) / aspect;
    result(2,2) = farDist * oneOverDepth;
    result(3,2) = (-farDist * nearDist) * oneOverDepth;
    result(2,3) = 1;
    result(3,3) = 0;
}

CViewport::CViewport(QWidget * parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f), QOpenGLFunctions()
{
    
}

void CViewport::initializeGL()
{
    initializeOpenGLFunctions();
    
    glClearColor(0.0f, 0.58f, 1.0f, 1.0f);

    // Generate a Vertex Array Object (VAO).
    // This stores the "state" of the attributes for vertices:
    // when you set up how position/colour/normal/etc. are specified
    // in the buffers, the VAO remembers this format in order to
    // easily re-apply it later. In this sense it's sort of like
    // a macro.
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Generate and populate a buffer of vertex position data.
    glGenBuffers(1, &vertexbuffer);                 // Generates a buffer and records its handle in vertexbuffer.
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);    // Sets the array buffer target to use this buffer.

    // Sets up the buffer currently bound to GL_ARRAY_BUFFER, which as above is the vertex buffer we generated.
    // The size is 9 * sizeof(float), to cater for all 9 vertices, and the data is initialised from
    // the static vertex data for the triangle. The drawing mode hint is set to static because the triangle
    // geometry will not change.
    glBufferData(GL_ARRAY_BUFFER, VERTEX_DATA_SIZE, g_vertex_buffer_data, GL_STATIC_DRAW);

    // Record handles to our vertex and fragment shaders.
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    char const * VertexSourcePointer = vertexShader;

    // This function is a bit weird because it caters to multiple strings.
    // The third and fourth parameters are arrays of char pointers and ints,
    // and they both have the same number of elements (specified by parameter 2).
    // All of these strings are concatenated and become the shader source code.
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);

    // Compile!
    glCompileShader(VertexShaderID);

    // Get the results of the compilation in case of errors.
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    qDebug() << "Vertex shader compilation messages:" << &VertexShaderErrorMessage[0];

    // Same game for fragment shader.
    char const * FragmentSourcePointer = fragmentShader;
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check results.
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    qDebug() << "Fragment shader compilation messages:" << &FragmentShaderErrorMessage[0];

    // Link the program
    ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( qMax(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    qDebug() << "Shader program linking messages:" <<  &ProgramErrorMessage[0];

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    QMatrix4x4 Projection;
    ComputeFOVProjection(Projection, 45.0f, 4.0f/3.0f, 0.1f, 100.0f);

    QMatrix4x4 View(1,0,0,4,
                    0,1,0,3,
                    0,0,-1,3,
                    0,0,0,1);

    //MVP = Projection * View;
    MVP(0,0) = 0.5f;
    glUseProgram(ProgramID);
    MatrixID = glGetUniformLocation(ProgramID, "MVP");
}

void CViewport::resizeGL(int w, int h)
{
    
}


void CViewport::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, MVP.constData());

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableVertexAttribArray(0);
}
