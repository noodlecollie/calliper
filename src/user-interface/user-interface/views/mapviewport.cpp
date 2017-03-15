#include "mapviewport.h"
#include "renderer/opengl/openglhelpers.h"
#include "renderer/opengl/openglerrors.h"
#include "model/global/resourceenvironment.h"
#include "model/scenerenderer/scenerenderer.h"

namespace UserInterface
{
    MapViewport::MapViewport(QWidget *parent, Qt::WindowFlags f)
        : QOpenGLWidget(parent, f),
          m_pDataModel()
    {
    }

    MapViewport::~MapViewport()
    {
    }

    void MapViewport::loadDataModel(const QSharedPointer<Model::BaseFileDataModel> &model)
    {
        m_pDataModel = model.dynamicCast<Model::MapFileDataModel>();
    }

    void MapViewport::initializeGL()
    {
        GL_CURRENT_F;

        GLTRY(f->glEnable(GL_CULL_FACE));
        GLTRY(f->glCullFace(GL_BACK));

        GLTRY(f->glEnable(GL_DEPTH_TEST));
        GLTRY(f->glDepthFunc(GL_LESS));
    }

    void MapViewport::paintGL()
    {
        if ( !m_pDataModel )
        {
            return;
        }

        Model::SceneRenderer sceneRenderer(m_pDataModel->scene(), m_pDataModel->renderModel());
        sceneRenderer.setShaderPalette(Model::ResourceEnvironment::globalInstance()->shaderPaletteStore()
                                       ->shaderPalette(Model::ShaderPaletteStore::SimpleLitTexturedRenderMode));

        GL_CURRENT_F;
        GLTRY(f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        sceneRenderer.render(m_pDataModel->scene()->defaultCamera());
    }

    void MapViewport::resizeGL(int w, int h)
    {
        if ( !m_pDataModel )
        {
            return;
        }

        Model::SceneCamera* camera = m_pDataModel->scene()->defaultCamera();

        Model::CameraLens lens = camera->lens();
        lens.setAspectRatio(static_cast<float>(w)/static_cast<float>(h));
        camera->setLens(lens);
    }
}
