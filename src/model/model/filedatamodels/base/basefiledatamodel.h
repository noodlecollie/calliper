#ifndef BASEFILEDATAMODEL_H
#define BASEFILEDATAMODEL_H

#include "model_global.h"
#include <QString>

namespace Model
{
    class MODELSHARED_EXPORT BaseFileDataModel
    {
    public:
        virtual ~BaseFileDataModel();

        enum FileType
        {
            UnknownFile = 0,
            MapFile
        };

        // Returns what type this model actually is.
        virtual FileType type() const = 0;

    protected:
        BaseFileDataModel();
    };
}

#endif // BASEFILEDATAMODEL_H
