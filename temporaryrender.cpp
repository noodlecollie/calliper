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
#include "scene.h"
#include "sceneobject.h"
#include "camera.h"
#include "geometryfactory.h"
#include <QMatrix4x4>
#include "callipermath.h"
#include <QtMath>

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

Scene* scene = NULL;
SceneObject* block = NULL;
Camera* camera = NULL;

QMatrix4x4 blockRot = Math::matrixRotateZ(qDegreesToRadians(45.0f));

void temporarySetup(QOpenGLContext *context, QOpenGLFunctions_4_1_Core *f)
{
    resourceManager()->setLiveContext(context);

    // Set up geometry to render.
//    geometry = new GeometryData();
//    geometry->appendVertex(QVector3D(-0.8f, 0.0f, -0.8f), QVector3D(0,0,1), QVector2D(0,0));
//    geometry->appendVertex(QVector3D(0.8f, 0.0f, -0.8f), QVector3D(0,0,1), QVector2D(1,0));
//    geometry->appendVertex(QVector3D(0.8f, 0.0f, 0.8f), QVector3D(0,0,1), QVector2D(1,1));
//    geometry->appendVertex(QVector3D(-0.8f, 0.0f, 0.8f), QVector3D(0,0,1), QVector2D(0,1));
//    geometry->appendIndex(0);
//    geometry->appendIndex(1);
//    geometry->appendIndex(2);
//    geometry->appendIndex(0);
//    geometry->appendIndex(2);
//    geometry->appendIndex(3);
//    geometry->setTexture(0, "/textures/uvsample");

    scene = new Scene();
    block = new SceneObject(scene->root());
    camera = new Camera(scene->root());

    block->setGeometry(GeometryFactory::cube(0.1f));
    block->setPosition(QVector3D(0, 0.5f, -0.2f));
    block->geometry()->setTexture(0, "/textures/test");

    // Set rendering colour.
    renderer()->setGlobalColor(QColor(255,0,0));
    renderer()->setShaderIndex(2);

    resourceManager()->setLiveContext(NULL);
}

void temporaryRender(QOpenGLContext *context, QOpenGLFunctions_4_1_Core *f)
{
    resourceManager()->setLiveContext(context);

    // Bind geometry for rendering
//    geometry->upload();
//    geometry->bindVertices(true);
//    geometry->bindIndices(true);
    block->geometry()->upload();
    block->geometry()->bindVertices(true);
    block->geometry()->bindIndices(true);

    ShaderProgram* pr = resourceManager()->shader(renderer()->shaderIndex());
    pr->setModelToWorld(block->localToParent() * blockRot);
    pr->setWorldToCamera(camera->localToParent());
    pr->setCameraProjection(camera->lens().projectionMatrix());

    // Apply the desired shader, setting up vertex format.
    pr->apply();

    // Bind the textures for use.
//    QOpenGLTexture* tex = resourceManager()->texture(geometry->texture(0));
    QOpenGLTexture* tex = resourceManager()->texture(block->geometry()->texture(0));
    tex->bind(0);

    // Draw the geometry.
//    geometry->draw();
    block->geometry()->draw();

    // Release the shader.
    pr->release();

    resourceManager()->setLiveContext(NULL);
}
