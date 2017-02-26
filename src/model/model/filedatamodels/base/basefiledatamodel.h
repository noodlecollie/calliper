#ifndef BASEFILEDATAMODEL_H
#define BASEFILEDATAMODEL_H

#include "model_global.h"
#include <QString>
#include <QObject>

namespace Model
{
    class MODELSHARED_EXPORT BaseFileDataModel
    {
        Q_GADGET
    public:
        virtual ~BaseFileDataModel();

        enum ModelType
        {
            UnknownModel = 0,
            MapModel
        };

        // Returns what type this model actually is.
        virtual ModelType type() const = 0;

    protected:
        BaseFileDataModel();
    };
}

#endif // BASEFILEDATAMODEL_H
