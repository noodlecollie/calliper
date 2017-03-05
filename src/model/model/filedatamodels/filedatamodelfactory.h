#ifndef FILEDATAMODELFACTORY_H
#define FILEDATAMODELFACTORY_H

#include "model_global.h"
#include "base/basefiledatamodel.h"

namespace Model
{
    class MODELSHARED_EXPORT FileDataModelFactory
    {
    public:
        static BaseFileDataModel* createModel(BaseFileDataModel::ModelType type);

    private:
        FileDataModelFactory() = delete;
    };
}

#endif // FILEDATAMODELFACTORY_H
