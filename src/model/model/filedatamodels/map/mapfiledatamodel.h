#ifndef MAPFILEDATAMODEL_H
#define MAPFILEDATAMODEL_H

#include "model_global.h"
#include "model/filedatamodels/base/basefiledatamodel.h"
#include "model/scene/mapscene.h"
#include <QScopedPointer>

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

    private:
        QScopedPointer<MapScene> m_pScene;
    };
}

#endif // MAPFILEDATAMODEL_H
