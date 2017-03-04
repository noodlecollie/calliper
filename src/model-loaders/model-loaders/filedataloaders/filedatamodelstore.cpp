#include "filedatamodelstore.h"
#include <QFileInfo>
#include "model-loaders/filedataloaders/fileextensiondatamodelmap.h"
#include "model/filedatamodels/filedatamodelfactory.h"
#include "model-loaders/filedataloaders/filedataloaderfactory.h"
#include <QPair>
#include "calliperutil/general/generalutil.h"

namespace
{
    inline void setErrorString(QString* errorString, const QString& msg)
    {
        if ( errorString )
        {
            *errorString = msg;
        }
    }

    inline QString dataModelEnumString(Model::BaseFileDataModel::ModelType type)
    {
        return CalliperUtil::General::enumKeyFromValue<Model::BaseFileDataModel>("ModelType", type);
    }

    inline QString dataModelLoaderEnumString(ModelLoaders::BaseFileLoader::LoaderType type)
    {
        return CalliperUtil::General::enumKeyFromValue<ModelLoaders::BaseFileLoader>("LoaderType", type);
    }
}

namespace ModelLoaders
{
    class FileDataModelStore::FileDataModelInfo : public QPair<Model::BaseFileDataModel::ModelType, BaseFileLoader::LoaderType>
    {
        typedef QPair<Model::BaseFileDataModel::ModelType, BaseFileLoader::LoaderType> TypePair;

    public:
        FileDataModelInfo(Model::BaseFileDataModel::ModelType modelType, BaseFileLoader::LoaderType loaderType)
            : TypePair(modelType, loaderType)
        {
        }

        inline Model::BaseFileDataModel::ModelType& modelType()
        {
            return first;
        }

        inline const Model::BaseFileDataModel::ModelType& modelType() const
        {
            return first;
        }

        inline BaseFileLoader::LoaderType& loaderType()
        {
            return second;
        }

        inline const BaseFileLoader::LoaderType& loaderType() const
        {
            return second;
        }
    };

    FileDataModelStore::FileDataModelStore()
    {

    }

    QSharedPointer<Model::BaseFileDataModel> FileDataModelStore::dataModel(const QString &path) const
    {
        return m_FileModels.value(path, DataModelPointer());
    }

    bool FileDataModelStore::isFileLoaded(const QString &path) const
    {
        return m_FileModels.contains(path);
    }

    void FileDataModelStore::unloadFile(const QString &path)
    {
        m_FileModels.remove(path);
    }

    QStringList FileDataModelStore::loadedFiles() const
    {
        return m_FileModels.keys();
    }

    FileDataModelStore::ConstIterator FileDataModelStore::constBegin() const
    {
        return m_FileModels.constBegin();
    }

    FileDataModelStore::ConstIterator FileDataModelStore::constEnd() const
    {
        return m_FileModels.constEnd();
    }

    ModelLoaders::BaseFileLoader::SuccessCode FileDataModelStore::loadFile(const QString &path, QString *errorString)
    {
        if ( m_FileModels.contains(path) )
        {
            return BaseFileLoader::Success;
        }

        QString extension = QFileInfo(path).suffix();

        FileDataModelInfo info = getDataModelInfo(path);
        if ( info.loaderType() == BaseFileLoader::UnknownLoader )
        {
            setErrorString(errorString, QString("Unknown file format '%1'.").arg(extension));
            return BaseFileLoader::Failure;
        }

        if ( info.modelType() == Model::BaseFileDataModel::UnknownModel )
        {
            setErrorString(errorString, QString("File format '%1' not registered to any known file type.").arg(extension));
            return BaseFileLoader::Failure;
        }

        DataModelPointer dataModel(Model::FileDataModelFactory::createModel(info.modelType()));
        if ( dataModel.isNull() )
        {
            setErrorString(errorString,
                           QString("Unable to create underlying data model for file of type '%1' (from extension '%2').")
                                .arg(dataModelEnumString(info.modelType()))
                                .arg(extension));

            return BaseFileLoader::Failure;
        }

        QScopedPointer<ModelLoaders::BaseFileLoader> loader(ModelLoaders::FileDataLoaderFactory::createLoader(info.loaderType(), dataModel.data()));
        if ( loader.isNull() )
        {
            setErrorString(errorString,
                           QString("Unable to create loader of type '%1' for file of type '%2' (from extension '%3').")
                                .arg(dataModelLoaderEnumString(info.loaderType()))
                                .arg(dataModelEnumString(info.modelType()))
                                .arg(extension));

            return BaseFileLoader::Failure;
        }

        BaseFileLoader::SuccessCode success = loader->load(path, errorString);
        if ( success == BaseFileLoader::Failure )
        {
            return BaseFileLoader::Failure;
        }

        m_FileModels.insert(path, dataModel);
        return success;
    }

    FileDataModelStore::FileDataModelInfo FileDataModelStore::getDataModelInfo(const QString &path) const
    {
        ModelLoaders::FileExtensionDataModelMap extMap;
        QString extension = QFileInfo(path).suffix();
        BaseFileLoader::LoaderType loaderType = extMap.loaderTypeForExtension(extension);
        return FileDataModelInfo(extMap.modelType(loaderType), loaderType);
    }
}
