#ifndef VMFDATAMODEL_H
#define VMFDATAMODEL_H

#include "app-calliper_global.h"
#include <QObject>
#include <QScopedPointer>
#include "model/scene/mapscene.h"
#include "renderer/rendermodel/0-modellevel/rendermodel.h"
#include "model/scenerenderer/simplerenderpassclassifier.h"
#include "model/scenerenderer/scenerenderer.h"

namespace AppCalliper
{
    class VmfDataModel : public QObject
    {
        Q_OBJECT
    public:
        VmfDataModel();
        ~VmfDataModel();

        void draw();

    private:
        QScopedPointer<Renderer::RenderModel> m_pRenderModel;
        QScopedPointer<Model::MapScene> m_pScene;
        Model::SimpleRenderPassClassifier m_RenderPassClassifier;
        Model::SceneRenderer m_SceneRenderer;
    };
}

#endif // VMFDATAMODEL_H
