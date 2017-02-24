#include "basefileloader.h"
#include "model/filedatamodels/base/basefiledatamodel.h"

namespace ModelLoaders
{
    BaseFileLoader::BaseFileLoader(Model::BaseFileDataModel *dataModel)
        : m_pDataModel(dataModel)
    {
        Q_ASSERT_X(m_pDataModel, Q_FUNC_INFO, "Expected a valid data model");
    }

    Model::BaseFileDataModel* BaseFileLoader::dataModel()
    {
        return m_pDataModel;
    }

    const Model::BaseFileDataModel* BaseFileLoader::dataModel() const
    {
        return m_pDataModel;
    }
}
