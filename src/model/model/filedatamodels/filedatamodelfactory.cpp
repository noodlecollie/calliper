#include "filedatamodelfactory.h"
#include "map/mapfiledatamodel.h"

namespace Model
{
    BaseFileDataModel* FileDataModelFactory::createModel(BaseFileDataModel::FileType type)
    {
        switch ( type )
        {
            case BaseFileDataModel::MapFile:
            {
                return new MapFileDataModel();
            }

            default:
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Model type was unrecognised");
                return nullptr;
            }
        }
    }
}
