#include "fileextensiondatamodelmap.h"

namespace ModelLoaders
{
    bool FileExtensionDataModelMap::m_bInitialised = false;
    QHash<QString, BaseFileLoader::FileType> FileExtensionDataModelMap::m_ExtensionToFileType;
    QHash<BaseFileLoader::FileType, Model::BaseFileDataModel::FileType> FileExtensionDataModelMap::m_FileTypeToDataModelType;

    FileExtensionDataModelMap::FileExtensionDataModelMap()
    {
        if ( m_bInitialised )
        {
            return;
        }

        addExtension("vmf", BaseFileLoader::VmfFile, Model::BaseFileDataModel::MapFile);
    }

    BaseFileLoader::FileType FileExtensionDataModelMap::fileType(const QString &extension) const
    {
        return m_ExtensionToFileType.value(extension.toLower(), BaseFileLoader::UnknownFile);
    }

    Model::BaseFileDataModel::FileType FileExtensionDataModelMap::dataModelType(BaseFileLoader::FileType fileType) const
    {
        return m_FileTypeToDataModelType.value(fileType, Model::BaseFileDataModel::UnknownFile);
    }

    Model::BaseFileDataModel::FileType FileExtensionDataModelMap::dataModelType(const QString &extension) const
    {
        return dataModelType(fileType(extension));
    }

    void FileExtensionDataModelMap::addExtension(const QString &extension,
                                                 BaseFileLoader::FileType fileType,
                                                 Model::BaseFileDataModel::FileType dataModelType)
    {
        Q_ASSERT_X(fileType != BaseFileLoader::UnknownFile && dataModelType != Model::BaseFileDataModel::UnknownFile,
                   Q_FUNC_INFO,
                   "Cannot add entry for unknown file type!");

        if ( fileType == BaseFileLoader::UnknownFile || dataModelType == Model::BaseFileDataModel::UnknownFile )
        {
            return;
        }

        Q_ASSERT_X(!m_ExtensionToFileType.contains(extension.toLower()),
                   Q_FUNC_INFO,
                   "File extension already has a registered file type!");

        Q_ASSERT_X(!m_FileTypeToDataModelType.contains(fileType),
                   Q_FUNC_INFO,
                   "File type already has a registered data model type!");

        m_ExtensionToFileType[extension.toLower()] = fileType;
        m_FileTypeToDataModelType[fileType] = dataModelType;
    }
}
