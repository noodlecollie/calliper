#ifndef BASEFILELOADER_H
#define BASEFILELOADER_H

#include "model-loaders_global.h"

namespace Model
{
    class BaseFileDataModel;
}

namespace ModelLoaders
{
    class MODELLOADERSSHARED_EXPORT BaseFileLoader
    {
    public:
        enum FileType
        {
            UnknownType = 0,
            VmfFile
        };

        enum SuccessCode
        {
            Failure = 0,
            PartialSuccess,
            Success,
        };

        virtual FileType type() const = 0;

        Model::BaseFileDataModel* dataModel();
        const Model::BaseFileDataModel* dataModel() const;

        virtual SuccessCode load(const QString& filePath, QString* errorString = nullptr) = 0;
        virtual SuccessCode save(const QString& filePath, QString* errorString = nullptr) = 0;

    protected:
        BaseFileLoader(Model::BaseFileDataModel* dataModel);

        Model::BaseFileDataModel* m_pDataModel;
    };
}

#endif // BASEFILELOADER_H
