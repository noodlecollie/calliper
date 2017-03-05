#include "filedatamodelfactory.h"
#include "map/mapfiledatamodel.h"

namespace Model
{
    BaseFileDataModel* FileDataModelFactory::createModel(BaseFileDataModel::ModelType type)
    {
        switch ( type )
        {
            case BaseFileDataModel::MapModel:
            {
                return new MapFileDataModel();
            }

            default:
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Model type was unrecognised");
                return Q_NULLPTR;
            }
        }
    }
}
