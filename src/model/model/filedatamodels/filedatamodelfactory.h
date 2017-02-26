#ifndef FILEDATAMODELFACTORY_H
#define FILEDATAMODELFACTORY_H

#include "model_global.h"
#include "base/basefiledatamodel.h"

namespace Model
{
    class FileDataModelFactory
    {
    public:
        static BaseFileDataModel* createModel(BaseFileDataModel::ModelType type);

    private:
        FileDataModelFactory() = delete;
    };
}

#endif // FILEDATAMODELFACTORY_H
