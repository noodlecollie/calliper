#ifndef APPLICATIONPROJECT_H
#define APPLICATIONPROJECT_H

#include "app-calliper_global.h"
#include "model/projects/calliperproject.h"
#include "model/core/datachangenotifier.h"
#include "model-loaders/filedataloaders/filedatamodelstore.h"

namespace AppCalliper
{
    class ApplicationProject : public Model::DataChangeNotifier
    {
        Q_OBJECT
    public:
        explicit ApplicationProject(QObject* parent = 0);

        QString fileName() const;
        void setFileName(const QString& filename);

        Model::CalliperProject* project();
        const Model::CalliperProject* project() const;

        ModelLoaders::FileDataModelStore& fileStore();
        const ModelLoaders::FileDataModelStore& fileStore() const;

    private:
        typedef QSharedPointer<Model::BaseFileDataModel> DataModelPointer;

        QString m_strFileName;
        Model::CalliperProject* m_pProject;
        ModelLoaders::FileDataModelStore m_FileStore;
    };
}

#endif // APPLICATIONPROJECT_H
