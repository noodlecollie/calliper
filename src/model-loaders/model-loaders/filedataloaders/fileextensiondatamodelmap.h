#ifndef FILEEXTENSIONDATAMODELMAP_H
#define FILEEXTENSIONDATAMODELMAP_H

#include "model-loaders_global.h"
#include "model-loaders/filedataloaders/base/basefileloader.h"
#include "model/filedatamodels/base/basefiledatamodel.h"
#include <QHash>

namespace ModelLoaders
{
    class FileExtensionDataModelMap
    {
    public:
        FileExtensionDataModelMap();

        BaseFileLoader::FileType fileType(const QString& extension) const;
        Model::BaseFileDataModel::FileType dataModelType(BaseFileLoader::FileType fileType) const;
        Model::BaseFileDataModel::FileType dataModelType(const QString& extension) const;

    private:
        void addExtension(const QString& extension, BaseFileLoader::FileType fileType, Model::BaseFileDataModel::FileType dataModelType);

        static QHash<QString, BaseFileLoader::FileType> m_ExtensionToFileType;
        static QHash<BaseFileLoader::FileType, Model::BaseFileDataModel::FileType> m_FileTypeToDataModelType;
        static bool m_bInitialised;
    };
}

#endif // FILEEXTENSIONDATAMODELMAP_H
