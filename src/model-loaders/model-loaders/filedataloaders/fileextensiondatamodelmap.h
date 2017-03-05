#ifndef FILEEXTENSIONDATAMODELMAP_H
#define FILEEXTENSIONDATAMODELMAP_H

#include "model-loaders_global.h"
#include <QObject>
#include "model-loaders/filedataloaders/base/basefileloader.h"
#include "model/filedatamodels/base/basefiledatamodel.h"
#include <QHash>

namespace ModelLoaders
{
    class MODELLOADERSSHARED_EXPORT FileExtensionDataModelMap
    {
        Q_GADGET
    public:
        FileExtensionDataModelMap();

        QList<BaseFileLoader::LoaderType> supportedLoaderTypes() const;
        BaseFileLoader::LoaderType loaderTypeForExtension(const QString& extension) const;
        bool isLoaderTypeSupported(BaseFileLoader::LoaderType loaderType) const;

        // Description string returned is translated.
        QString description(BaseFileLoader::LoaderType loaderType) const;
        QStringList extensions(BaseFileLoader::LoaderType loaderType) const;
        Model::BaseFileDataModel::ModelType modelType(BaseFileLoader::LoaderType loaderType) const;
        Model::BaseFileDataModel::ModelType modelType(const QString& extension) const;

        // Description strings returned are translated.
        QString fileDialogTypeString(BaseFileLoader::LoaderType loaderType) const;
        QStringList fileDialogTypeStrings() const;

    private:
        struct FileInfo
        {
            BaseFileLoader::LoaderType m_iLoaderType;
            QString m_strDescription;
            QStringList m_Extensions;
            Model::BaseFileDataModel::ModelType m_iModelType;

            FileInfo(BaseFileLoader::LoaderType loaderType,
                     const QString& description,
                     const QStringList& extensions,
                     Model::BaseFileDataModel::ModelType modelType)
                : m_iLoaderType(loaderType),
                  m_strDescription(description),
                  m_Extensions(extensions),
                  m_iModelType(modelType)
            {
            }

            FileInfo()
                : FileInfo(BaseFileLoader::UnknownLoader,
                           QString(),
                           QStringList(),
                           Model::BaseFileDataModel::UnknownModel)
            {
            }
        };

        void initialise();
        void addFile(BaseFileLoader::LoaderType loaderType,
                     const QString& description,
                     const QStringList& extensions,
                     Model::BaseFileDataModel::ModelType modelType);

        static bool m_bInitialised;
        static QHash<BaseFileLoader::LoaderType, FileInfo> m_FileInfo;
        static QHash<QString, BaseFileLoader::LoaderType> m_ExtensionToLoaderType;
    };
}

#endif // FILEEXTENSIONDATAMODELMAP_H
