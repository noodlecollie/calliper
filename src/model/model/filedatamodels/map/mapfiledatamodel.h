#ifndef MAPFILEDATAMODEL_H
#define MAPFILEDATAMODEL_H

#include "model_global.h"
#include <QScopedPointer>

#include "model/filedatamodels/base/basefiledatamodel.h"
#include "model/scene/mapscene.h"

#include "rendersystem/interface-classes/definitions/publicrendermodeldefs.h"

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

        RenderSystem::PublicRenderModelDefs::RenderModelId renderModelId() const;
        void setRenderModelId(RenderSystem::PublicRenderModelDefs::RenderModelId renderModelId);

    private:
        QScopedPointer<MapScene> m_pScene;
        RenderSystem::PublicRenderModelDefs::RenderModelId m_nRenderModelId;
    };
}

#endif // MAPFILEDATAMODEL_H
