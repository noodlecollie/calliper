#ifndef IMODELVIEW_H
#define IMODELVIEW_H

#include "user-interface_global.h"
#include <QWeakPointer>

namespace Model
{
    class BaseFileDataModel;
}

class QWidget;

namespace UserInterface
{
    // We have a slight problem here - we'd generally like views to be QWidgets,
    // but we'd also like to enforce that they have certain functions, eg. being
    // able to set a data model to view. We can't just create a subclass of
    // QWidget for this purpose, as anything else that we might like to use that
    // derives from QWidget cannot then be used as a view because it won't
    // dervice from this custom class. Instead we provice IModelView as an
    // interface, and enforce that the view can represent itself as a QWidget.
    class IModelView
    {
    public:
        virtual ~IModelView() {}

        virtual QWidget* modelViewToWidget() = 0;
        virtual const QWidget* modelViewToWidget() const = 0;

        // We don't want views to store full QSharedPointers by default, as the data model
        // should be clearly owned by something else.
        virtual QWeakPointer<Model::BaseFileDataModel> dataModel() const = 0;
        virtual void setDataModel(const QWeakPointer<Model::BaseFileDataModel>& model) = 0;
    };
}

#endif // IMODELVIEW_H
