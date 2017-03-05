#include "modelviewfactory.h"
#include "user-interface/views/mapviewport.h"

namespace UserInterface
{
    QWidget* ModelViewFactory::createView(Model::BaseFileDataModel::ModelType modelType)
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
