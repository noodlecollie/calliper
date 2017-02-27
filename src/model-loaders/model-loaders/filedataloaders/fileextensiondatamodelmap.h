#ifndef FILEEXTENSIONDATAMODELMAP_H
#define FILEEXTENSIONDATAMODELMAP_H

#include "model-loaders_global.h"
#include "model-loaders/filedataloaders/base/basefileloader.h"
#include "model/filedatamodels/base/basefiledatamodel.h"
#include <QHash>

namespace ModelLoaders
{
    class MODELLOADERSSHARED_EXPORT FileExtensionDataModelMap
    {
    public:
        FileExtensionDataModelMap();

        QStringList supportedExtensions() const;

        BaseFileLoader::LoaderType loaderType(const QString& extension) const;
        Model::BaseFileDataModel::ModelType modelType(BaseFileLoader::LoaderType fileType) const;
        Model::BaseFileDataModel::ModelType modelType(const QString& extension) const;
        QString description(const QString& extension) const;

    private:
        void addExtension(const QString& extension,
                          const QString& description,
                          BaseFileLoader::LoaderType loaderType,
                          Model::BaseFileDataModel::ModelType modelType);

        static QHash<QString, QString> m_ExtensionToDescription;
        static QHash<QString, BaseFileLoader::LoaderType> m_ExtensionToFileType;
        static QHash<BaseFileLoader::LoaderType, Model::BaseFileDataModel::ModelType> m_FileTypeToDataModelType;
        static bool m_bInitialised;
    };
}

#endif // FILEEXTENSIONDATAMODELMAP_H
