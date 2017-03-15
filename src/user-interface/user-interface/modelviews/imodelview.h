#ifndef IMODELVIEW_H
#define IMODELVIEW_H

#include "user-interface_global.h"
#include <QSharedPointer>

namespace Model
{
    class BaseFileDataModel;
}

namespace UserInterface
{
    // TODO: Move to model
    class IModelView
    {
    public:
        virtual ~IModelView() {}

        virtual void loadDataModel(const QSharedPointer<Model::BaseFileDataModel>& model) = 0;
    };
}

#endif // IMODELVIEW_H
