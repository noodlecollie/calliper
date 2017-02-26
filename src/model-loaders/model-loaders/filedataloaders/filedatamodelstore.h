#ifndef FILEDATAMODELSTORE_H
#define FILEDATAMODELSTORE_H

#include "model-loaders_global.h"
#include <QSharedPointer>
#include "model/filedatamodels/base/basefiledatamodel.h"
#include "model-loaders/filedataloaders/base/basefileloader.h"
#include <QHash>
#include <QString>

namespace ModelLoaders
{
    class FileDataModelStore
    {
    public:
        FileDataModelStore();

        ModelLoaders::BaseFileLoader::SuccessCode loadFile(const QString& localPath, QString* errorString = nullptr);
        QSharedPointer<Model::BaseFileDataModel> dataModel(const QString& localPath) const;

    private:
        class FileDataModelInfo;
        typedef QSharedPointer<Model::BaseFileDataModel> DataModelPointer;

        FileDataModelInfo getDataModelInfo(const QString& path) const;
        DataModelPointer createDataModel(const QString& path, QString* errorString);

        QHash<QString, DataModelPointer> m_FileModels;
    };
}

#endif // FILEDATAMODELSTORE_H
