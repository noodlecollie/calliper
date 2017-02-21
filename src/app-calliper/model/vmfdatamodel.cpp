#include "vmfdatamodel.h"
#include "model/global/resourceenvironment.h"

namespace AppCalliper
{
    VmfDataModel::VmfDataModel()
        : m_pRenderModel(new Renderer::RenderModel()),
          m_pScene(new Model::MapScene()),
          m_RenderPassClassifier(),
          m_SceneRenderer(&m_RenderPassClassifier, m_pRenderModel.data(), m_pScene.data())
    {
        m_SceneRenderer.setShaderPalette(Model::ResourceEnvironment::globalInstance()->failsafeShaderPalette());
    }

    VmfDataModel::~VmfDataModel()
    {
        m_pScene.reset();
        m_pRenderModel.reset();
    }

    void VmfDataModel::draw()
    {
        m_SceneRenderer.render(m_pScene->defaultCamera());
    }
}
