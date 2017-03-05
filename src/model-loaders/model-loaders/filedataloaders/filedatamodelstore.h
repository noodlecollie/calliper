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
    class MODELLOADERSSHARED_EXPORT FileDataModelStore
    {
    public:
        typedef QHash<QString, QSharedPointer<Model::BaseFileDataModel> >::const_iterator ConstIterator;

        FileDataModelStore();

        ModelLoaders::BaseFileLoader::SuccessCode loadFile(const QString& path, QString* errorString = Q_NULLPTR);
        QSharedPointer<Model::BaseFileDataModel> dataModel(const QString& path) const;
        bool isFileLoaded(const QString& path) const;

        // If the shared pointer is held by other objects, the model itself will not be
        // destroyed until the shared pointers are released. It will however be removed
        // from this object.
        void unloadFile(const QString& path);

        QStringList loadedFiles() const;
        ConstIterator constBegin() const;
        ConstIterator constEnd() const;

    private:
        class FileDataModelInfo;
        typedef QSharedPointer<Model::BaseFileDataModel> DataModelPointer;

        FileDataModelInfo getDataModelInfo(const QString& path) const;

        QHash<QString, DataModelPointer> m_FileModels;
    };
}

#endif // FILEDATAMODELSTORE_H
