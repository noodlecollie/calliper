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

void OpenGLRenderer::preRender()
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
    program->setUniformMatrix4(ShaderProgram::CoordinateTransformMatrix, Math::hammerToOpenGL());
}

void OpenGLRenderer::setOneOffUniforms(ShaderProgram *program, const QMatrix4x4 &camera, const QMatrix4x4 &projection)
{
    program->setUniformMatrix4(ShaderProgram::WorldToCameraMatrix, camera);
    program->setUniformMatrix4(ShaderProgram::CameraProjectionMatrix, projection);
}

void OpenGLRenderer::postRender()
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
    GeometryData* geometry = GeometryFactory::fullScreenQuad();
    geometry->setLocalTexture(QSharedPointer<QOpenGLTexture>(tex));

    return geometry;
}
