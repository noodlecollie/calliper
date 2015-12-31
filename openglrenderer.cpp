#include "openglrenderer.h"
#include <QOpenGLFunctions_4_1_Core>
#include "resourcemanager.h"
#include "sceneobject.h"
#include "shaderprogram.h"
#include "callipermath.h"
#include "camera.h"
#include <QOpenGLTexture>
#include "scene.h"
#include <QPainter>
#include "geometrydata.h"
#include "geometryfactory.h"

static OpenGLRenderer* g_pRenderer = NULL;
OpenGLRenderer* renderer()
{
    return g_pRenderer;
}

OpenGLRenderer::OpenGLRenderer()
{
    g_pRenderer = this;
    m_pPainter = NULL;
    m_bPreparedForRendering = false;
    m_vecDirectionalLight = QVector3D(1,0,0);
    m_colFogColour = QColor::fromRgb(0xff999999);
    m_flFogBegin = 100;
    m_flFogEnd = 1000;
}

OpenGLRenderer::~OpenGLRenderer()
{
    resourceManager()->makeCurrent();
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    // Nothing yet
    Q_UNUSED(f);

    resourceManager()->doneCurrent();
}

void OpenGLRenderer::initialise()
{
    Q_ASSERT(!g_pRenderer);
    new OpenGLRenderer();
}

void OpenGLRenderer::shutdown()
{
    Q_ASSERT(g_pRenderer);
    delete g_pRenderer;
    g_pRenderer = NULL;
}

QColor OpenGLRenderer::globalColor() const
{
    return m_colGlobalColour;
}

void OpenGLRenderer::setGlobalColor(const QColor &col)
{
    m_colGlobalColour = col;
}

void OpenGLRenderer::setUpOpenGLResources()
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    // Nothing yet.
    Q_UNUSED(f);

    setShaderIndex(0);
}

int OpenGLRenderer::shaderIndex() const
{
    return m_iShader;
}

void OpenGLRenderer::setShaderIndex(int index)
{
    m_iShader = index;
}

void OpenGLRenderer::begin()
{
    ShaderProgram* program = resourceManager()->shader(shaderIndex());
    Q_ASSERT(program);

    // Start with autoupdate false so that we can apply things in a batch.
    m_pPainter = new OpenGLPainter(program, false);

    // Set up initial things.
    m_pPainter->coordinateTransformPostMultiply(Math::hammerToOpenGL());
    m_pPainter->fogColorSetTop(fogColor());
    m_pPainter->fogBeginSetTop(fogBeginDistance());
    m_pPainter->fogEndSetTop(fogEndDistance());
    m_pPainter->directionalLightSetTop(directionalLight());
    m_pPainter->globalColorSetTop(globalColor());

    // Apply them all.
    m_pPainter->applyAll();
    m_pPainter->setAutoUpdate(true);

    m_bPreparedForRendering = true;
}

void OpenGLRenderer::end()
{
    Q_ASSERT(m_bPreparedForRendering);

    Q_ASSERT(m_pPainter->inInitialState());
    delete m_pPainter;
    m_pPainter = NULL;

    m_bPreparedForRendering = false;
}

void OpenGLRenderer::renderSceneRecursive(SceneObject *obj, OpenGLPainter* painter)
{
    painter->modelToWorldPush();

    obj->draw(painter);

    QList<SceneObject*> children = obj->children();
    foreach ( SceneObject* o, children )
    {
        renderSceneRecursive(o, painter);
    }

    painter->modelToWorldPop();
}

QVector3D OpenGLRenderer::directionalLight() const
{
    return m_vecDirectionalLight;
}

void OpenGLRenderer::setDirectionalLight(const QVector3D &dir)
{
    m_vecDirectionalLight = dir.normalized();
}

void OpenGLRenderer::setDirectionalLight(const EulerAngle &ang)
{
    m_vecDirectionalLight = Math::angleToVectorSimple(ang);
}

void OpenGLRenderer::renderScene2(Scene *scene, const Camera *camera)
{
    Q_ASSERT(m_bPreparedForRendering);

    m_pPainter->setCamera(camera);
    m_pPainter->worldToCameraPostMultiply(camera->rootToLocal());
    m_pPainter->cameraProjectionPostMultiply(camera->lens().projectionMatrix());

    // Render the scene.
    renderSceneRecursive(scene->root(), m_pPainter);
}

GeometryData* OpenGLRenderer::createTextQuad(const QSize &texSize, const QString &text, const QColor &col,
                                                              const QFont &font, Qt::Alignment alignment)
{
    Q_ASSERT(texSize.width() > 0 && Math::isPowerOfTwo(texSize.width()));
    Q_ASSERT(texSize.height() > 0 && Math::isPowerOfTwo(texSize.height()));

    QImage image(texSize, QImage::Format_ARGB32);

    // Make the image transparent.
    image.fill(0x00000000);

    // Draw on the image.
    {
        QPainter painter(&image);
        painter.setPen(col);
        painter.setFont(font);
        painter.drawText(image.rect(), alignment, text);
    }

    // Create a texture with the image.
    QOpenGLTexture* tex = new QOpenGLTexture(image.mirrored(), QOpenGLTexture::DontGenerateMipMaps);
    tex->setMagnificationFilter(QOpenGLTexture::Nearest);
    tex->setMinificationFilter(QOpenGLTexture::Nearest);
    tex->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);
    tex->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);

    // Create the actual quad.
    // The vertices are specified in OpenGL co-ordinates.
    GeometryData* geometry = GeometryFactory::triangleQuad(1);
    geometry->setLocalTexture(QSharedPointer<QOpenGLTexture>(tex));

    return geometry;
}

void OpenGLRenderer::drawQuad(GeometryData *quad, const QSize &screen, const QRect &subrect, Qt::Alignment alignment,
                              int offset, int count)
{
    Q_ASSERT(m_bPreparedForRendering);

    // We transform the quad as follows:
    // - Translate by (transX,transY,0). transX and transY depend on the alignment passed in, but will be one of -1, 0 or 1.
    // - Scale by (0.5,-0,5,1). This makes the bottom right of the quad at (1,1).
    // - Scale by (width, height, 1). This makes the quad our desired size.
    // - Translate by (x,y). This puts the top left of the quad at our desired co-ordinates.
    // - Convert from window to device co-ordinates.

    float transX = 0, transY = 0;

    if ( alignment.testFlag(Qt::AlignLeft) )
    {
        transX++;
    }

    if ( alignment.testFlag(Qt::AlignRight) )
    {
        transX--;
    }

    if ( alignment.testFlag(Qt::AlignTop) )
    {
        transY--;
    }

    if ( alignment.testFlag(Qt::AlignBottom) )
    {
        transY++;
    }

//    QMatrix4x4 transformation = Math::windowToDevice(screen.width(), screen.height())
//            * Math::matrixTranslate(QVector3D(subrect.x(), subrect.y(), 0))
//            * Math::matrixScale(QVector3D(subrect.width(),subrect.height(),1))
//            * Math::matrixScale(QVector3D(0.5f,-0.5f,1))
//            * Math::matrixTranslate(QVector3D(transX,transY,0));

    float x = subrect.x(), y = subrect.y(), w = subrect.width(), h = subrect.height();
    QMatrix4x4 transformation(0.5f*w, 0, 0, (0.5f*w*transX)+x,
                              0, -0.5f*h, 0, (-0.5f*h*transY)+y,
                              0, 0, 1, 0,
                              0, 0, 0, 1);

    transformation = Math::windowToDevice(screen.width(), screen.height()) * transformation;

    quad->upload();
    quad->bindVertices(true);
    quad->bindIndices(true);

    m_pPainter->modelToWorldPostMultiply(transformation);
    m_pPainter->worldToCameraSetToIdentity();
    m_pPainter->coordinateTransformSetToIdentity();
    m_pPainter->cameraProjectionSetToIdentity();
    quad->applyDataFormat(m_pPainter->shaderTop());

    QOpenGLTexture* tex = quad->hasLocalTexture() ? quad->localTexture().data()
                                                  : resourceManager()->texture(quad->texture(0));
    tex->bind(0);

    quad->draw(offset, count);
}

QColor OpenGLRenderer::fogColor() const
{
    return m_colFogColour;
}

void OpenGLRenderer::setFogColor(const QColor &col)
{
    m_colFogColour = col;
}

float OpenGLRenderer::fogBeginDistance() const
{
    return m_flFogBegin;
}

void OpenGLRenderer::setFogBeginDistance(float dist)
{
    m_flFogBegin = dist;
}

float OpenGLRenderer::fogEndDistance() const
{
    return m_flFogEnd;
}

void OpenGLRenderer::setFogEndDistance(float dist)
{
    m_flFogEnd = dist;
}
