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

    QWidget* MapViewport::modelViewToWidget()
    {
        return this;
    }

    const QWidget* MapViewport::modelViewToWidget() const
    {
        return this;
    }

    QWeakPointer<Model::BaseFileDataModel> MapViewport::dataModel() const
    {
        return m_pDataModel;
    }

    void MapViewport::setDataModel(const QWeakPointer<Model::BaseFileDataModel> &model)
    {
        QSharedPointer<Model::MapFileDataModel> mapModel = qSharedPointerDynamicCast<Model::MapFileDataModel>(model);
        if ( mapModel.isNull() )
        {
            m_pDataModel = QWeakPointer<Model::MapFileDataModel>();
        }

        m_pDataModel = mapModel.toWeakRef();
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
        MapFileDataModelPointer mapModel = m_pDataModel.toStrongRef();
        if ( !mapModel )
        {
            return;
        }

        Model::SceneRenderer sceneRenderer(mapModel->scene(), mapModel->renderModel());
        sceneRenderer.setShaderPalette(Model::ResourceEnvironment::globalInstance()->shaderPaletteStore()
                                       ->shaderPalette(Model::ShaderPaletteStore::SimpleLitTexturedRenderMode));

        GL_CURRENT_F;
        GLTRY(f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        sceneRenderer.render(mapModel->scene()->defaultCamera());
    }

    void MapViewport::resizeGL(int w, int h)
    {
        MapFileDataModelPointer mapModel = m_pDataModel.toStrongRef();
        if ( !mapModel )
        {
            return;
        }

        Model::SceneCamera* camera = mapModel->scene()->defaultCamera();

        Model::CameraLens lens = camera->lens();
        lens.setAspectRatio(static_cast<float>(w)/static_cast<float>(h));
        camera->setLens(lens);
    }
}
