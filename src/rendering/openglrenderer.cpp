#include "openglrenderer.h"
#include <QOpenGLFunctions_4_1_Core>
#include "resourcemanager.h"
#include "sceneobject.h"
#include "shaderprogram.h"
#include "callipermath.h"
#include "scenecamera.h"
#include <QOpenGLTexture>
#include "mapscene.h"
#include <QPainter>
#include "geometrydata.h"
#include "geometryfactory.h"
#include <QSurface>
#include "shaders.h"
#include <QOpenGLTexture>
#include "iraydetectable.h"

void OpenGLRenderer::ObjectPicker::checkDrawnObject(SceneObject *obj)
{
    // Get the depth component and see if it's nearer than our current.
    float newDepth = 1.0f;
    functions->glReadPixels(selectionPos.x(), selectionPos.y(), 1, 1,
                            GL_DEPTH_COMPONENT,
                            GL_FLOAT,
                            &newDepth);

    if ( newDepth < nearestDepth )
    {
        nearestDepth = newDepth;
        selectedObject = obj;

        if ( getPickColour )
        {
            unsigned int rgba;
            functions->glReadPixels(selectionPos.x(), selectionPos.y(), 1, 1,
                                    GL_RGBA,
                                    GL_UNSIGNED_BYTE,
                                    &rgba);

            // As Qt sees it, this value is returned as ABGR.
            // We fix this here.
            selectedColour = (rgba & 0xff00ff00) |
                    ((rgba & 0x000000ff) << 16) |
                    ((rgba & 0x00ff0000) >> 16);
        }
    }
}

static OpenGLRenderer* g_pRenderer = NULL;
OpenGLRenderer* renderer()
{
    return g_pRenderer;
}

OpenGLRenderer::OpenGLRenderer()
{
    g_pRenderer = this;
    m_pStack = NULL;
    m_bPreparedForRendering = false;
    m_vecDirectionalLight = Math::angleToVectorSimple(EulerAngle(25.0f, -90.0f - 45.0f, 0.0f));
    m_colFogColour = QColor::fromRgb(0xff999999);
    m_colGlobalColour = QColor::fromRgb(0xffffffff);
    m_flFogBegin = 0;
    m_flFogEnd = 0;
    m_iShader = 0;
    m_bPicking = false;
    m_pRayTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
}

OpenGLRenderer::~OpenGLRenderer()
{
    resourceManager()->makeCurrent();
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();
    Q_UNUSED(f);

    delete m_pRayTexture;

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
    m_pStack = new ShaderStack(program, false);

    // Set up initial things.
    m_pStack->coordinateTransformPostMultiply(Math::hammerToOpenGL());
    m_pStack->fogColorSetTop(fogColor());
    m_pStack->fogBeginSetTop(fogBeginDistance());
    m_pStack->fogEndSetTop(fogEndDistance());
    m_pStack->directionalLightSetTop(directionalLight());
    m_pStack->globalColorSetTop(globalColor());

    // Apply them all.
    m_pStack->applyAll();
    m_pStack->setAutoUpdate(true);

    m_vecCurrentCameraWorldPosition = QVector3D();
    const HierarchicalObject* o = m_pStack->cameraParams().hierarchicalObject();
    if ( o )
    {
        m_vecCurrentCameraWorldPosition = (o->rootToLocal().inverted() * QVector4D(0,0,0,1)).toVector3D();
    }

    m_bPreparedForRendering = true;
}

void OpenGLRenderer::end()
{
    Q_ASSERT(m_bPreparedForRendering);

    Q_ASSERT(m_pStack->inInitialState());
    delete m_pStack;
    m_pStack = NULL;

    m_bPreparedForRendering = false;
}

void OpenGLRenderer::renderDeferred()
{
    renderTranslucent();
    clearDeferred();
}

void OpenGLRenderer::renderTranslucent()
{
    for ( QMap<float,DeferredObject>::const_iterator it = m_TranslucentObjects.constBegin();
          it != m_TranslucentObjects.constEnd(); ++it )
    {
        const DeferredObject &dfo = *it;

        m_pStack->modelToWorldPush();

        m_pStack->modelToWorldSetToIdentity();
        m_pStack->modelToWorldPreMultiply(dfo.matrix);
        dfo.object->draw(m_pStack);
        if ( m_bPicking )
            m_ObjectPicker.checkDrawnObject(dfo.object);

        m_pStack->modelToWorldPop();
    }
}

void OpenGLRenderer::renderSceneRecursive(SceneObject *obj, ShaderStack *stack)
{
    // If the object is hidden, don't draw it or any of its children.
    if ( obj->hidden() )
        return;

    stack->modelToWorldPush();
    stack->modelToWorldPostMultiply(obj->localToParent());

    // Check if we need to defer this object.
    bool deferred = false;
    if ( (obj->renderFlags() & SceneObject::Translucent) == SceneObject::Translucent )
    {
        deferred = true;

        // Order objects within the map by depth.
        // obj->position() is used because localToParent() has not yet been applied.
        // The depth is negated because of the way QMap orderes keys.
        QVector3D worldPos = ((stack->worldToCameraTop() * stack->modelToWorldTop()) * QVector4D(obj->position(), 1)).toVector3D();
        m_TranslucentObjects.insertMulti(-(worldPos - m_vecCurrentCameraWorldPosition).lengthSquared(), DeferredObject(obj, stack->modelToWorldTop()));
    }

    if ( !deferred )
    {
        obj->draw(stack);
        if ( m_bPicking )
            m_ObjectPicker.checkDrawnObject(obj);
    }

    QList<SceneObject*> children = obj->children();
    foreach ( SceneObject* o, children )
    {
        renderSceneRecursive(o, stack);
    }

    stack->modelToWorldPop();
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

void OpenGLRenderer::renderScene(BaseScene *scene, const CameraParams &params)
{
    Q_ASSERT(m_bPreparedForRendering);
    Q_ASSERT(m_pStack->worldToCameraTop().isIdentity());
    Q_ASSERT(m_pStack->cameraProjectionTop().isIdentity());

    m_pStack->setCameraParams(params);

    m_pStack->worldToCameraPush();
    m_pStack->worldToCameraPostMultiply(params.worldToCameraMatrix());

    m_pStack->cameraProjectionPush();
    m_pStack->cameraProjectionPostMultiply(params.projectionMatrix());

    m_pStack->globalColorPush();
    m_pStack->globalColorSetTop(globalColor());

    // Render the scene.
    renderSceneRecursive(scene->root(), m_pStack);

    // Render any deferred things we have left.
    renderDeferred();

    m_pStack->globalColorPop();
    m_pStack->cameraProjectionPop();
    m_pStack->worldToCameraPop();
}

SceneObject* OpenGLRenderer::selectFromDepthBuffer(BaseScene *scene, const CameraParams &params,
                                                   const QPoint &oglPos, QRgb *pickColor)
{
    Q_ASSERT(m_bPreparedForRendering);

    QOpenGLContext* context = QOpenGLContext::currentContext();
    Q_ASSERT(context);
    QOpenGLFunctions_4_1_Core* f = context->versionFunctions<QOpenGLFunctions_4_1_Core>();

    m_ObjectPicker = ObjectPicker(f, oglPos, pickColor != NULL);
    m_bPicking = true;

    m_pStack->setCameraParams(params);

    m_pStack->worldToCameraPush();
    m_pStack->worldToCameraPostMultiply(params.worldToCameraMatrix());

    m_pStack->cameraProjectionPush();
    m_pStack->cameraProjectionPostMultiply(params.projectionMatrix());

    m_pStack->shaderPush(resourceManager()->shader(SelectionMaskShader::staticName()));
    m_pStack->m_bLockShader = true;
    m_pStack->m_bPicking = true;

    f->glEnable(GL_SCISSOR_TEST);
    f->glScissor(oglPos.x(), oglPos.y(), 1, 1);

    renderSceneRecursive(scene->root(), m_pStack);
    renderDeferred();

    f->glDisable(GL_SCISSOR_TEST);
    m_pStack->m_bLockShader = false;
    m_pStack->m_bPicking = false;
    m_pStack->shaderPop();
    m_pStack->cameraProjectionPop();
    m_pStack->worldToCameraPop();
    m_bPicking = false;

    if ( pickColor )
    {
        *pickColor = m_ObjectPicker.selectedColour;
    }

    return m_ObjectPicker.selectedObject;
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

    m_pStack->modelToWorldPostMultiply(transformation);
    m_pStack->worldToCameraSetToIdentity();
    m_pStack->coordinateTransformSetToIdentity();
    m_pStack->cameraProjectionSetToIdentity();
    quad->applyDataFormat(m_pStack->shaderTop());

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

void OpenGLRenderer::clearDeferred()
{
    m_TranslucentObjects.clear();
}

QVector2D OpenGLRenderer::deviceCoordinates(const QVector3D &worldPos, const SceneCamera *camera)
{
    // World -> camera -> OpenGL camera -> projected -> divided by w and returned.
    return (camera->lens()->projectionMatrix() * Math::hammerToOpenGL() * camera->rootToLocal() * QVector4D(worldPos, 1)).toVector2DAffine();
}
