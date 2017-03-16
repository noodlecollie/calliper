#include "modelviewfactory.h"
#include "user-interface/views/mapviewport.h"
#include "user-interface/modelviews/imodelview.h"

namespace UserInterface
{
    IModelView* ModelViewFactory::createView(Model::BaseFileDataModel::ModelType modelType)
    {
        switch ( modelType )
        {
            case Model::BaseFileDataModel::MapModel:
            {
                return new MapViewport();
            }

            default:
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Model type was unrecognised.");
                return Q_NULLPTR;
            }
        }
    }
}
