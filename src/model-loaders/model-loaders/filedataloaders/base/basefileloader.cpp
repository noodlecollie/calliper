#include "basefileloader.h"
#include "model/filedatamodels/base/basefiledatamodel.h"

namespace ModelLoaders
{
    BaseFileLoader::BaseFileLoader()
        : m_pDataModel(nullptr)
    {
    }

    BaseFileLoader::~BaseFileLoader()
    {
    }

    Model::BaseFileDataModel* BaseFileLoader::dataModel() const
    {
        return m_pDataModel;
    }

    bool BaseFileLoader::isValid() const
    {
        return m_pDataModel != nullptr;
    }
}
