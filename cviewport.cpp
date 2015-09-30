#include "cviewport.h"
#include <QtMath>
#include <QKeyEvent>

const GLfloat cube_vertices[] = {
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
};

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
        "float remap(float inp){while (inp>200.0){inp-=200.0;} return inp/200.0;}\n"
        "void main(){\n"
        "color = vec3(remap(gl_FragCoord.x), remap(gl_FragCoord.y), 0);}\n"
        "//color = gl_Normal.xyz;}\n";

QMatrix4x4 perspectiveMatrix(float fov, float aspectRatio, float near, float far)
{
    float top = near * qTan((M_PI/180.0f) * (fov/2.0f));
    float bottom = -top;
    float right = top * aspectRatio;
    float left = -right;

    return QMatrix4x4((2.0f*near)/(right-left), 0, (right+left)/(right-left), 0,
                      0, (2.0f*near)/(top-bottom), (top+bottom)/(top-bottom), 0,
                      0, 0, -(far+near)/(far-near), -(2.0f*far*near)/(far-near),
                      0, 0, -1, 0);
}

QMatrix4x4 orthographicMatrix(float top, float bottom, float left, float right, float near, float far)
{
    return QMatrix4x4((2.0f)/(right-left), 0, 0, -(right+left)/(right-left),
                      0, (2.0f)/(top-bottom), 0, -(top+bottom)/(top-bottom),
                      0, 0, -(2.0f)/(far-near), -(far+near)/(far-near),
                      0, 0, 0, 1);
}

CViewport::CViewport(QWidget * parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f), QOpenGLFunctions()
{
    Top = 2;
    Bottom = -2;
    Left = -2;
    Right = 2;
    Near = 0.01f;
    Far = 10;

    setFocusPolicy(Qt::StrongFocus);
    usePerspective = false;
}

void CViewport::initializeGL()
{
    initializeOpenGLFunctions();
    
    glClearColor(0.0f, 0.58f, 1.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

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
    //glBufferData(GL_ARRAY_BUFFER, VERTEX_DATA_SIZE, g_vertex_buffer_data, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 3 * 2 * 6, cube_vertices, GL_STATIC_DRAW);

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

    //qDebug() << "Projection:" << Projection << "MVP:" << MVP;
    glUseProgram(ProgramID);

    QMatrix4x4 A(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    QMatrix4x4 B(16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1);
    qDebug() << "AB =" << (A*B) << "BA =" << (B*A);

//    QVector4D v0(g_vertex_buffer_data[0], g_vertex_buffer_data[1], g_vertex_buffer_data[2], 1);
//    QVector4D v1(g_vertex_buffer_data[3], g_vertex_buffer_data[4], g_vertex_buffer_data[5], 1);
//    QVector4D v2(g_vertex_buffer_data[6], g_vertex_buffer_data[7], g_vertex_buffer_data[8], 1);

//    v0 = MVP * v0;
//    v1 = MVP * v1;
//    v2 = MVP * v2;

//    qDebug() << "Transformed v0:" << v0 << "clip," << QVector3D(v0.x()/v0.w(), v0.y()/v0.w(), v0.z()/v0.w()) << "device.";
//    qDebug() << "Transformed v1:" << v1 << "clip," << QVector3D(v1.x()/v1.w(), v1.y()/v1.w(), v1.z()/v1.w()) << "device.";
//    qDebug() << "Transformed v2:" << v2 << "clip," << QVector3D(v2.x()/v2.w(), v2.y()/v2.w(), v2.z()/v2.w()) << "device.";
}

void CViewport::resizeGL(int w, int h)
{
    update();
}


void CViewport::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 Projection = usePerspective ? perspectiveMatrix(45.0f, (float)width()/(float)height(), Near, Far)
                                           : orthographicMatrix(Top, Bottom, Left, Right, Near, Far);

    QMatrix4x4 View(1,0,0,camPos.x(),
                    0,1,0,camPos.y(),
                    0,0,1,camPos.z(),
                    0,0,0,1);

    float deg = 30;
    float s45 = qSin(qRadiansToDegrees(deg));
    float c45 = qCos(qRadiansToDegrees(deg));
    QMatrix4x4 scaleDown(0.5f,0,0,0, 0,0.5f,0,0, 0,0,0.5f,0, 0,0,0,1);
    QMatrix4x4 rotateY(c45,0,s45,0, 0,1,0,0, -s45,0,c45,0, 0,0,0,1);
    QMatrix4x4 rotateX(1,0,0,0, 0,c45,-s45,0, 0,s45,c45,0, 0,0,0,1);
    QMatrix4x4 model = rotateX * rotateY * scaleDown;
    MVP = Projection * View * model;
    MatrixID = glGetUniformLocation(ProgramID, "MVP");

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
    //glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6);

    glDisableVertexAttribArray(0);
}

void CViewport::keyPressEvent(QKeyEvent *e)
{
    switch ( e->key() )
    {
    case Qt::Key_W:
        camPos.setZ(camPos.z() - 0.1f);
        qDebug() << camPos;
        update();
        return;

    case Qt::Key_S:
        camPos.setZ(camPos.z() + 0.1f);
        qDebug() << camPos;
        update();
        return;

    case Qt::Key_A:
        camPos.setX(camPos.x() - 0.1f);
        qDebug() << camPos;
        update();
        return;

    case Qt::Key_D:
        camPos.setX(camPos.x() + 0.1f);
        qDebug() << camPos;
        update();
        return;

    case Qt::Key_Q:
        camPos.setY(camPos.y() + 0.1f);
        qDebug() << camPos;
        update();
        return;

    case Qt::Key_Z:
        camPos.setY(camPos.y() - 0.1f);
        qDebug() << camPos;
        update();
        return;

    case Qt::Key_Space:
        usePerspective = !usePerspective;
        qDebug() << "Use perspective:" << usePerspective;
        update();
        return;

    default:
        QOpenGLWidget::keyPressEvent(e);
        return;
    }
}
