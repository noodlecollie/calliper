#include "temporaryrender.h"
#include <QtDebug>
#include "resourcemanager.h"
#include "shaderprogram.h"
#include "openglrenderer.h"
#include <QOpenGLBuffer>
#include "geometrydata.h"
#include <QByteArray>
#include <QImage>
#include <QOpenGLTexture>

QByteArray convertImage(const QString &filename, int &width, int &height)
{
    QImage image = QImage(filename).mirrored();
    const uchar* imgData = image.constBits();
    int numImagePixels = image.width() * image.height();
    width = image.width();
    height = image.height();
    QByteArray arr(sizeof(uint) * numImagePixels, 0);

    for ( int i = 0; i < numImagePixels; i++ )
    {
        // Get an integer representing 4 bytes of image data.
        uint data = ((const uint*)imgData)[i];

        // Swap the bytes round.
        uchar* d = (uchar*)&data;
        uchar alpha = d[3];
        d[3] = d[2];
        d[2] = d[1];
        d[1] = d[0];
        d[0] = alpha;

        ((uint*)arr.data())[i] = data;
    }

    return arr;
}

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

const float texPixels[] = {
    0.0f, 0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f, 1.0f,
};

GLuint VertexArrayID = 0;
GLuint vertexbuffer = 0;
GLuint indexbuffer = 0;
GLuint VertexShaderID = 0;
GLuint FragmentShaderID = 0;
GLuint ProgramID = 0;
GLuint locColour = 0;
GLuint TextureID = 0;
ShaderProgram* shader = NULL;

QOpenGLBuffer* pVertexBuffer = NULL;
QOpenGLBuffer* pIndexBuffer = NULL;
GeometryData* geometry = NULL;
QOpenGLTexture* glTex = NULL;

void temporarySetup(QOpenGLContext *context, QOpenGLFunctions_4_1_Core *f)
{
    resourceManager()->setLiveContext(context);

    // Set up geometry to render.
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

//    int width = 0, height = 0;
//    QByteArray texture = convertImage(":/textures/test.png", width, height);

//    f->glGenTextures(1, &TextureID);
//    f->glActiveTexture(GL_TEXTURE0);
//    f->glBindTexture(GL_TEXTURE_2D, TextureID);
//    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, texture.constData());
//    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);        // Wrapping
//    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);    // Sampling
//    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    f->glGenerateMipmap(GL_TEXTURE_2D);

    glTex = new QOpenGLTexture(QImage(":/textures/test.png").mirrored());
    glTex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    glTex->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    glTex->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    glTex->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    glTex->generateMipMaps();

    // Set rendering colour.
    renderer()->setGlobalColor(QColor(255,0,0));
    renderer()->setShaderIndex(1);

    resourceManager()->setLiveContext(NULL);
}

void temporaryRender(QOpenGLContext *context, QOpenGLFunctions_4_1_Core *f)
{
    resourceManager()->setLiveContext(context);

    // Bind geometry for rendering
    geometry->bindVertices(true);
    geometry->bindIndices(true);

    // Apply the desired shader, setting up vertex format.
    resourceManager()->shader(renderer()->shaderIndex())->apply();

//    f->glActiveTexture(GL_TEXTURE0 + 0);
//    f->glEnable(GL_TEXTURE_2D);
//    f->glBindTexture(GL_TEXTURE_2D, TextureID);
//    GLuint i = glGetUniformLocation(resourceManager()->shader(renderer()->shaderIndex())->handle(), "tex");
//    f->glUniform1i(i, 0);

    glTex->bind();

    // Draw the geometry.
    geometry->draw();

    // Release the shader.
    resourceManager()->shader(renderer()->shaderIndex())->apply();

    resourceManager()->setLiveContext(NULL);
}
