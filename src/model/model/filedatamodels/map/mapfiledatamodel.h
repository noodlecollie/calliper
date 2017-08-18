#ifndef MAPFILEDATAMODEL_H
#define MAPFILEDATAMODEL_H

#include "model_global.h"
#include <QScopedPointer>

#include "model/filedatamodels/base/basefiledatamodel.h"
#include "model/scene/mapscene.h"

#include "rendersystem/interface-classes/definitions/rendermodeldefs.h"

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

        RenderSystem::RenderModelDefs::RenderModelId renderModelId() const;

        void setRenderModelName(const QString& name) const;

    private:
        QScopedPointer<MapScene> m_pScene;
        RenderSystem::RenderModelDefs::RenderModelId m_nRenderModelId;
    };
}

#endif // MAPFILEDATAMODEL_H
