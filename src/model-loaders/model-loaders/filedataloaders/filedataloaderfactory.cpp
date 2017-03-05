#include "filedataloaderfactory.h"
#include "model-loaders/filedataloaders/vmf/vmfdataloader.h"

namespace ModelLoaders
{
    BaseFileLoader* FileDataLoaderFactory::createLoader(BaseFileLoader::LoaderType loaderType, Model::BaseFileDataModel *dataModel)
    {
        BaseFileLoader* loader = Q_NULLPTR;

        switch ( loaderType )
        {
            case BaseFileLoader::VmfLoader:
            {
                loader = new VmfDataLoader();
                break;
            }

            default:
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "File type was unrecognised");
                return Q_NULLPTR;
            }
        }

        if ( !loader->setDataModel(dataModel) )
        {
            delete loader;
            return Q_NULLPTR;
        }

        return loader;
    }
}
