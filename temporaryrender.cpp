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
#include <QKeyEvent>
#include "cameracontroller.h"
#include <QTime>
#include <QMouseEvent>

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
CameraController cameraController;
QTime cameraDelta;
bool trackingMouse = false;
QPoint lastMousePos;

QMatrix4x4 blockRot = Math::matrixRotateZ(qDegreesToRadians(45.0f));

#define MOVEMENT_DELTA 0.01f
#define ANGLE_DELTA 10.0f
#define MOUSE_SENSITIVITY 2.0f

void temporarySetup(QOpenGLContext *context, QOpenGLFunctions_4_1_Core *f)
{
    scene = new Scene();
    block = new SceneObject(scene->root());
    camera = new Camera(scene->root());

    block->setGeometry(GeometryFactory::cube(16.0f));
    block->setPosition(QVector3D(64, 0, 0));
    block->setAngles(EulerAngle(0,45,0));
    block->geometry()->setTexture(0, "/textures/test");

    geometry = new GeometryData();
    geometry->setDrawMode(GL_LINES);
    geometry->appendVertex(QVector3D(0,0,0), QColor(255,0,0));
    geometry->appendVertex(QVector3D(64,0,0), QColor(255,0,0));
    geometry->appendVertex(QVector3D(0,0,0), QColor(0,255,0));
    geometry->appendVertex(QVector3D(0,64,0), QColor(0,255,0));
    geometry->appendVertex(QVector3D(0,0,0), QColor(0,0,255));
    geometry->appendVertex(QVector3D(0,0,64), QColor(0,0,255));
    geometry->appendIndex(0);
    geometry->appendIndex(1);
    geometry->appendIndex(2);
    geometry->appendIndex(3);
    geometry->appendIndex(4);
    geometry->appendIndex(5);

    // Set rendering colour.
    renderer()->setGlobalColor(QColor(255,0,0));
    renderer()->setShaderIndex(4);

    cameraController.setTopSpeed(10.0f);
    cameraDelta.start();
}

void temporaryRender(QOpenGLContext *context, QOpenGLFunctions_4_1_Core *f)
{
    int ms = cameraDelta.restart();
    cameraController.update(ms);
    float frac = (float)ms/1000.0f;
    camera->translate(cameraController.velocity()/* * frac*/);

    // Bind geometry for rendering
    geometry->upload();
    geometry->bindVertices(true);
    geometry->bindIndices(true);

    ShaderProgram* pr2 = resourceManager()->shader(3);
    pr2->apply();
//    pr2->setAttributeFormat(ShaderProgram::Position, 3, 7*sizeof(float), 0);
//    pr2->setAttributeFormat(ShaderProgram::Color, 4, 7*sizeof(float), 3*sizeof(float));
    geometry->applyDataFormat(pr2);
    pr2->setUniformMatrix4(ShaderProgram::ModelToWorldMatrix, QMatrix4x4());
    pr2->setUniformMatrix4(ShaderProgram::WorldToCameraMatrix, camera->parentToLocal());
    pr2->setUniformMatrix4(ShaderProgram::CoordinateTransformMatrix, Math::hammerToOpenGL());
    pr2->setUniformMatrix4(ShaderProgram::CameraProjectionMatrix, camera->lens().projectionMatrix());
    geometry->draw();
    pr2->release();

    block->geometry()->upload();
    block->geometry()->bindVertices(true);
    block->geometry()->bindIndices(true);

    ShaderProgram* pr = resourceManager()->shader(renderer()->shaderIndex());

    // Apply the desired shader.
    pr->apply();

//    pr->setAttributeFormat(ShaderProgram::Position, 3, 8*sizeof(float), 0);
//    pr->setAttributeFormat(ShaderProgram::UV, 2, 8*sizeof(float), 6*sizeof(float));
    block->geometry()->applyDataFormat(pr);
    pr->setUniformVector3(ShaderProgram::DirectionalLightUniform, QVector3D(1,0,-1).normalized());
    pr->setUniformMatrix4(ShaderProgram::ModelToWorldMatrix, block->localToParent());
    pr->setUniformMatrix4(ShaderProgram::WorldToCameraMatrix, camera->parentToLocal());
    pr->setUniformMatrix4(ShaderProgram::CoordinateTransformMatrix, Math::hammerToOpenGL());
    pr->setUniformMatrix4(ShaderProgram::CameraProjectionMatrix, camera->lens().projectionMatrix());

    // Bind the textures for use.
//    QOpenGLTexture* tex = resourceManager()->texture(geometry->texture(0));
    QOpenGLTexture* tex = resourceManager()->texture(block->geometry()->texture(0));
    tex->bind(0);

    // Draw the geometry.
    block->geometry()->draw();

    // Release the shader.
    pr->release();
}

bool temporaryKeyPress(QKeyEvent *e)
{
    if (!camera) return false;

    bool updated = false;
    EulerAngle ang = camera->angles();

    switch (e->key())
    {
    case Qt::Key_W:
        if ( e->isAutoRepeat() ) return false;
        cameraController.forward(true);
        updated = true;
        break;

    case Qt::Key_S:
        if ( e->isAutoRepeat() ) return false;
        cameraController.backward(true);
        updated = true;
        break;

    case Qt::Key_A:
        if ( e->isAutoRepeat() ) return false;
        cameraController.left(true);
        updated = true;
        break;

    case Qt::Key_D:
        if ( e->isAutoRepeat() ) return false;
        cameraController.right(true);
        updated = true;
        break;

    case Qt::Key_Up:
        ang.setPitch(ang.pitch() - ANGLE_DELTA);
        updated = true;
        break;

    case Qt::Key_Down:
        ang.setPitch(ang.pitch() + ANGLE_DELTA);
        updated = true;
        break;

    case Qt::Key_Left:
        ang.setYaw(ang.yaw() + ANGLE_DELTA);
        updated = true;
        break;

    case Qt::Key_Right:
        ang.setYaw(ang.yaw() - ANGLE_DELTA);
        updated = true;
        break;
    }

    camera->setAngles(ang);
    return updated;
}

bool temporaryKeyRelease(QKeyEvent *e)
{
   if (!camera) return false;

   bool updated = false;

   switch (e->key())
   {
   case Qt::Key_W:
       cameraController.forward(false);
       updated = true;
       break;

   case Qt::Key_S:
       cameraController.backward(false);
       updated = true;
       break;

   case Qt::Key_A:
       cameraController.left(false);
       updated = true;
       break;

   case Qt::Key_D:
       cameraController.right(false);
       updated = true;
       break;

   default:
       break;
   }

   return updated;
}

bool temporaryMousePress(QMouseEvent *e)
{
    if ( e->button() == Qt::LeftButton )
    {
        trackingMouse = true;
        lastMousePos = e->pos();
    }
    return false;
}

bool temporaryMouseMove(QMouseEvent *e)
{
    if ( !trackingMouse )
        return false;

    QPoint delta = e->pos() - lastMousePos;
    EulerAngle angles = camera->angles();
    angles.setPitch(angles.pitch() + ((float)delta.y() * MOUSE_SENSITIVITY));
    angles.setYaw(angles.yaw() - ((float)delta.x() * MOUSE_SENSITIVITY));
    camera->setAngles(angles);
    lastMousePos = e->pos();
    return true;
}

bool temporaryMouseRelease(QMouseEvent *e)
{
    if ( e->button() == Qt::LeftButton )
    {
        trackingMouse = false;
    }
    return false;
}
