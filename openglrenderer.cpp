#include "openglrenderer.h"
#include <QOpenGLFunctions_4_1_Core>
#include "resourcemanager.h"
#include "sceneobject.h"
#include "shaderprogram.h"
#include "callipermath.h"
#include "camera.h"
#include <QOpenGLTexture>
#include "scene.h"

static OpenGLRenderer* g_pRenderer = NULL;
OpenGLRenderer* renderer()
{
    return g_pRenderer;
}

OpenGLRenderer::OpenGLRenderer()
{
    g_pRenderer = this;
    m_vecDirectionalLight = QVector3D(1,0,0);
}

OpenGLRenderer::~OpenGLRenderer()
{
    resourceManager()->makeCurrent();
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    // Nothing yet

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

void OpenGLRenderer::renderSceneRecursive(SceneObject *obj, MatrixStack &stack,
                                          const QMatrix4x4 &worldToCam, const QMatrix4x4 &projection)
{
    bool pushed = false;
    if ( !obj->geometry()->isEmpty() )
    {
        pushed = true;
        stack.push();
        stack.top() = stack.top() * obj->localToParent();

        obj->geometry()->upload();
        obj->geometry()->bindVertices(true);
        obj->geometry()->bindIndices(true);

        ShaderProgram* program = resourceManager()->shader(shaderIndex());
        program->apply();

        obj->geometry()->applyDataFormat(program);
        program->setUniformVector3(ShaderProgram::DirectionalLightUniform, m_vecDirectionalLight);
        program->setUniformMatrix4(ShaderProgram::ModelToWorldMatrix, stack.top());
        program->setUniformMatrix4(ShaderProgram::WorldToCameraMatrix, worldToCam);
        program->setUniformMatrix4(ShaderProgram::CoordinateTransformMatrix, Math::hammerToOpenGL());
        program->setUniformMatrix4(ShaderProgram::CameraProjectionMatrix, projection);

        QOpenGLTexture* tex = resourceManager()->texture(obj->geometry()->texture(0));
        tex->bind(0);

        obj->geometry()->draw();

        program->release();
    }

    QList<SceneObject*> children = obj->children();
    foreach ( SceneObject* o, children )
    {
        renderSceneRecursive(o, stack, worldToCam, projection);
    }

    if (pushed)
    {
        stack.pop();
    }
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
    MatrixStack stack;
    renderSceneRecursive(scene->root(), stack, camera->rootToLocal(), camera->lens().projectionMatrix());
}
