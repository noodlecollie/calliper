#include "fileextensiondatamodelmap.h"
#include <QCoreApplication>

namespace
{
    inline QString translate(const QString& text)
    {
        return QCoreApplication::translate(
                    ModelLoaders::FileExtensionDataModelMap::staticMetaObject.className(),
                    qPrintable(text));
    }
}

namespace ModelLoaders
{
    bool FileExtensionDataModelMap::m_bInitialised = false;
    QHash<BaseFileLoader::LoaderType, FileExtensionDataModelMap::FileInfo> FileExtensionDataModelMap::m_FileInfo;
    QHash<QString, BaseFileLoader::LoaderType> FileExtensionDataModelMap::m_ExtensionToLoaderType;

    FileExtensionDataModelMap::FileExtensionDataModelMap()
    {
        initialise();
    }

    QList<BaseFileLoader::LoaderType> FileExtensionDataModelMap::supportedLoaderTypes() const
    {
        return m_FileInfo.keys();
    }

    BaseFileLoader::LoaderType FileExtensionDataModelMap::loaderTypeForExtension(const QString &extension) const
    {
        return m_ExtensionToLoaderType.value(extension.toLower(), BaseFileLoader::UnknownLoader);
    }

    bool FileExtensionDataModelMap::isLoaderTypeSupported(BaseFileLoader::LoaderType loaderType) const
    {
        return m_FileInfo.contains(loaderType);
    }

    QString FileExtensionDataModelMap::description(BaseFileLoader::LoaderType loaderType) const
    {
        if ( !isLoaderTypeSupported(loaderType) )
        {
            return QString();
        }

        return translate(m_FileInfo[loaderType].m_strDescription);
    }

    QStringList FileExtensionDataModelMap::extensions(BaseFileLoader::LoaderType loaderType) const
    {
        if ( !isLoaderTypeSupported(loaderType) )
        {
            return QStringList();
        }

        return m_FileInfo[loaderType].m_Extensions;
    }

    Model::BaseFileDataModel::ModelType FileExtensionDataModelMap::modelType(BaseFileLoader::LoaderType loaderType) const
    {
        if ( !isLoaderTypeSupported(loaderType) )
        {
            return Model::BaseFileDataModel::UnknownModel;
        }

        return m_FileInfo[loaderType].m_iModelType;
    }

    QString FileExtensionDataModelMap::fileDialogTypeString(BaseFileLoader::LoaderType loaderType) const
    {
        if ( !isLoaderTypeSupported(loaderType) )
        {
            return QString();
        }

        const FileInfo& fileInfo = m_FileInfo[loaderType];

        // Format: "JPEG File (*.jpg *.jpeg)"
        return QString("%1 (*.%2)")
                .arg(translate(fileInfo.m_strDescription))
                .arg(fileInfo.m_Extensions.join(" *."));
    }

    QStringList FileExtensionDataModelMap::fileDialogTypeStrings() const
    {
        QStringList list;
        QList<BaseFileLoader::LoaderType> loaderTypes = m_FileInfo.keys();

        foreach ( BaseFileLoader::LoaderType type, loaderTypes )
        {
            list.append(fileDialogTypeString(type));
        }

        return list;
    }

    void FileExtensionDataModelMap::initialise()
    {
        if ( m_bInitialised )
        {
            return;
        }

        addFile(BaseFileLoader::VmfLoader, "Valve Map File", QStringList() << "vmf", Model::BaseFileDataModel::MapModel);

        m_bInitialised = true;
    }

    void FileExtensionDataModelMap::addFile(BaseFileLoader::LoaderType loaderType,
                                            const QString &description,
                                            const QStringList &extensions,
                                            Model::BaseFileDataModel::ModelType modelType)
    {
        QStringList lowercaseExtensions;
        foreach ( const QString& ext, extensions )
        {
            QString lowercase = ext.toLower();
            Q_ASSERT_X(!m_ExtensionToLoaderType.contains(lowercase), Q_FUNC_INFO, "Loader type already registered with this extension!");
            if ( m_ExtensionToLoaderType.contains(lowercase) )
            {
                return;
            }

            lowercaseExtensions.append(ext.toLower());
        }

        Q_ASSERT_X(loaderType != BaseFileLoader::UnknownLoader, Q_FUNC_INFO, "Expected valid loader type!");
        Q_ASSERT_X(!lowercaseExtensions.isEmpty(), Q_FUNC_INFO, "Expected at least one extension!");
        Q_ASSERT_X(modelType != Model::BaseFileDataModel::UnknownModel, Q_FUNC_INFO, "Expected valid model type!");

        m_FileInfo.insert(loaderType, FileInfo(loaderType, description, lowercaseExtensions, modelType));

        foreach ( const QString& ext, lowercaseExtensions )
        {
            m_ExtensionToLoaderType.insert(ext, loaderType);
        }
    }
}
