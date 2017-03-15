#ifndef MAPFILEDATAMODEL_H
#define MAPFILEDATAMODEL_H

#include "model_global.h"
#include <QScopedPointer>

#include "model/filedatamodels/base/basefiledatamodel.h"
#include "model/scene/mapscene.h"

#include "renderer/rendermodel/0-modellevel/rendermodel.h"

namespace Model
{
    class MODELSHARED_EXPORT MapFileDataModel : public BaseFileDataModel
    {
    public:
        MapFileDataModel();
        virtual ~MapFileDataModel();

        virtual ModelType type() const override;

        MapScene* scene();
        const MapScene* scene() const;

        Renderer::RenderModel* renderModel();
        const Renderer::RenderModel* renderModel() const;

    private:
        QScopedPointer<MapScene> m_pScene;
        Renderer::RenderModel m_RenderModel;
    };
}

#endif // MAPFILEDATAMODEL_H
