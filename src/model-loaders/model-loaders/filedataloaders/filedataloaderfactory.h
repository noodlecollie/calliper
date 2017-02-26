#ifndef FILEDATALOADERFACTORY_H
#define FILEDATALOADERFACTORY_H

#include "model-loaders_global.h"
#include "model-loaders/filedataloaders/base/basefileloader.h"

namespace ModelLoaders
{
    class FileDataLoaderFactory
    {
    public:
        static BaseFileLoader* createLoader(BaseFileLoader::LoaderType loaderType, Model::BaseFileDataModel* dataModel);

    private:
        FileDataLoaderFactory() = delete;
    };
}

#endif // FILEDATALOADERFACTORY_H
