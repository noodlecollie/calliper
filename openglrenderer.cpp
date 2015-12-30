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
    m_pShaderProgram = NULL;
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
    m_pShaderProgram = resourceManager()->shader(shaderIndex());
    m_pShaderProgram->apply();

    // Uniforms:
    // ==== Set here:
    // - Co-ordinate transform
    // - Directional light
    //
    // ==== Set once by rendering function:
    // - World to camera matrix
    // - Camera projection matrix
    //
    // ==== Set many times throughout rendering:
    // - Model to world matrix

    setCommonUniforms(m_pShaderProgram);

    m_bPreparedForRendering = true;
}

void OpenGLRenderer::setCommonUniforms(ShaderProgram *program)
{
    program->setUniformVector3(ShaderProgram::DirectionalLightUniform, m_vecDirectionalLight);
    program->setUniformColor4(ShaderProgram::FogColorUniform, m_colFogColour);
    program->setUniformFloat(ShaderProgram::FogBeginUniform, m_flFogBegin);
    program->setUniformFloat(ShaderProgram::FogEndUniform, m_flFogEnd);
    program->setUniformMatrix4(ShaderProgram::CoordinateTransformMatrix, Math::hammerToOpenGL());
}

void OpenGLRenderer::setOneOffUniforms(ShaderProgram *program, const QMatrix4x4 &camera, const QMatrix4x4 &projection)
{
    program->setUniformMatrix4(ShaderProgram::WorldToCameraMatrix, camera);
    program->setUniformMatrix4(ShaderProgram::CameraProjectionMatrix, projection);
}

void OpenGLRenderer::end()
{
    m_pShaderProgram->release();
    m_pShaderProgram = NULL;

    m_bPreparedForRendering = false;
}

void OpenGLRenderer::renderSceneRecursive(SceneObject *obj, MatrixStack &stack,
                                          const QMatrix4x4 &camera, const QMatrix4x4 &projection)
{
    stack.push();
    stack.top() = stack.top() * obj->localToParent();

    if ( !obj->geometry()->isEmpty() )
    {
        obj->geometry()->upload();
        obj->geometry()->bindVertices(true);
        obj->geometry()->bindIndices(true);

        ShaderProgram* program = m_pShaderProgram;

        // If we have a shader override, set up the new shader.
        QString override = obj->geometry()->shaderOverride();
        if ( !override.isNull() )
        {
            ShaderProgram* pr = resourceManager()->shader(override);
            if ( pr )
            {
                program = pr;
                liveSwitchShader(m_pShaderProgram, program, camera, projection);
            }
        }

        // Render the actual object with our chosen shader.
        obj->geometry()->applyDataFormat(program);
        program->setUniformMatrix4(ShaderProgram::ModelToWorldMatrix, stack.top());

        QOpenGLTexture* tex = resourceManager()->texture(obj->geometry()->texture(0));
        tex->bind(0);

        obj->geometry()->draw();

        if ( m_pShaderProgram != program )
        {
            liveSwitchShader(program, m_pShaderProgram, camera, projection);
        }
    }

    QList<SceneObject*> children = obj->children();
    foreach ( SceneObject* o, children )
    {
        renderSceneRecursive(o, stack, camera, projection);
    }

    stack.pop();
}

void OpenGLRenderer::renderSceneRecursive(SceneObject *obj, OpenGLPainter &painter)
{
    painter.modelToWorldPush();

    obj->draw(&painter);

    QList<SceneObject*> children = obj->children();
    foreach ( SceneObject* o, children )
    {
        renderSceneRecursive(o, painter);
    }

    painter.modelToWorldPop();
}

void OpenGLRenderer::liveSwitchShader(ShaderProgram *oldShader, ShaderProgram *newShader,
                                      const QMatrix4x4 &camera, const QMatrix4x4 &projection)
{
    oldShader->release();
    newShader->apply();
    setCommonUniforms(newShader);
    setOneOffUniforms(newShader, camera, projection);
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

void OpenGLRenderer::renderScene(Scene *scene, const Camera *camera)
{
    Q_ASSERT(m_bPreparedForRendering);

    QMatrix4x4 cameraMatrix = camera->rootToLocal();
    QMatrix4x4 projectionMatrix = camera->lens().projectionMatrix();
    setOneOffUniforms(m_pShaderProgram, cameraMatrix, projectionMatrix);

    MatrixStack stack;
    renderSceneRecursive(scene->root(), stack, cameraMatrix, projectionMatrix);
}

void OpenGLRenderer::renderScene2(Scene *scene, const Camera *camera)
{
    Q_ASSERT(m_bPreparedForRendering);

    // Start with autoupdate false so that we can apply things in a batch.
    OpenGLPainter painter(m_pShaderProgram, false);

    // Set up initial things.
    painter.worldToCameraPostMultiply(camera->rootToLocal());
    painter.cameraProjectionPostMultiply(camera->lens().projectionMatrix());
    painter.coordinateTransformPostMultiply(Math::hammerToOpenGL());
    painter.fogColorSetTop(fogColor());
    painter.fogBeginSetTop(fogBeginDistance());
    painter.fogEndSetTop(fogEndDistance());
    painter.directionalLightSetTop(directionalLight());
    painter.globalColorSetTop(globalColor());

    // Apply them all.
    painter.applyAll();
    painter.setAutoUpdate(true);

    // Render the scene.
    renderSceneRecursive(scene->root(), painter);
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

    m_pShaderProgram->setUniformMatrix4(ShaderProgram::ModelToWorldMatrix, transformation);
    m_pShaderProgram->setUniformMatrix4(ShaderProgram::WorldToCameraMatrix, QMatrix4x4());
    m_pShaderProgram->setUniformMatrix4(ShaderProgram::CoordinateTransformMatrix, QMatrix4x4());
    m_pShaderProgram->setUniformMatrix4(ShaderProgram::CameraProjectionMatrix, QMatrix4x4());

    quad->applyDataFormat(m_pShaderProgram);

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
