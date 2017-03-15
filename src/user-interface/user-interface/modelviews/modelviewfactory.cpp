#include "modelviewfactory.h"
#include "user-interface/views/mapviewport.h"
#include "user-interface/modelviews/imodelview.h"

namespace UserInterface
{
    QWidget* ModelViewFactory::createView(Model::BaseFileDataModel::ModelType modelType)
    {
        QWidget* view = Q_NULLPTR;

        switch ( modelType )
        {
            case Model::BaseFileDataModel::MapModel:
            {
                view = new MapViewport();
                break;
            }

            default:
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Model type was unrecognised.");
                return Q_NULLPTR;
            }
        }

        IModelView* modelViewInterface = dynamic_cast<IModelView*>(view);
        Q_ASSERT_X(modelViewInterface, Q_FUNC_INFO, "Expected view to implement interface!");
        if ( !modelViewInterface )
        {
            delete view;
            return Q_NULLPTR;
        }

        return view;
    }
}
