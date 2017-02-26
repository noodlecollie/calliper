#include "fileextensiondatamodelmap.h"

namespace ModelLoaders
{
    bool FileExtensionDataModelMap::m_bInitialised = false;
    QHash<QString, BaseFileLoader::LoaderType> FileExtensionDataModelMap::m_ExtensionToFileType;
    QHash<BaseFileLoader::LoaderType, Model::BaseFileDataModel::ModelType> FileExtensionDataModelMap::m_FileTypeToDataModelType;

    FileExtensionDataModelMap::FileExtensionDataModelMap()
    {
        if ( m_bInitialised )
        {
            return;
        }

        addExtension("vmf", BaseFileLoader::VmfLoader, Model::BaseFileDataModel::MapModel);
    }

    BaseFileLoader::LoaderType FileExtensionDataModelMap::loaderType(const QString &extension) const
    {
        return m_ExtensionToFileType.value(extension.toLower(), BaseFileLoader::UnknownLoader);
    }

    Model::BaseFileDataModel::ModelType FileExtensionDataModelMap::modelType(BaseFileLoader::LoaderType loaderType) const
    {
        return m_FileTypeToDataModelType.value(loaderType, Model::BaseFileDataModel::UnknownModel);
    }

    Model::BaseFileDataModel::ModelType FileExtensionDataModelMap::modelType(const QString &extension) const
    {
        return modelType(loaderType(extension));
    }

    void FileExtensionDataModelMap::addExtension(const QString &extension,
                                                 BaseFileLoader::LoaderType loaderType,
                                                 Model::BaseFileDataModel::ModelType modelType)
    {
        Q_ASSERT_X(loaderType != BaseFileLoader::UnknownLoader && modelType != Model::BaseFileDataModel::UnknownModel,
                   Q_FUNC_INFO,
                   "Cannot add entry for unknown file type!");

        if ( loaderType == BaseFileLoader::UnknownLoader || modelType == Model::BaseFileDataModel::UnknownModel )
        {
            return;
        }

        Q_ASSERT_X(!m_ExtensionToFileType.contains(extension.toLower()),
                   Q_FUNC_INFO,
                   "File extension already has a registered loader type!");

        Q_ASSERT_X(!m_FileTypeToDataModelType.contains(loaderType),
                   Q_FUNC_INFO,
                   "Loader type already has a registered model type!");

        m_ExtensionToFileType[extension.toLower()] = loaderType;
        m_FileTypeToDataModelType[loaderType] = modelType;
    }
}
