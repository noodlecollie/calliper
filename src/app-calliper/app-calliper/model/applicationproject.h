#ifndef APPLICATIONPROJECT_H
#define APPLICATIONPROJECT_H

#include "model/projects/calliperproject.h"
#include "model/core/datachangenotifier.h"
#include "model-loaders/filedataloaders/filedatamodelstore.h"

class ApplicationProject : public Model::DataChangeNotifier
{
    Q_OBJECT
public:
    explicit ApplicationProject(QObject* parent = 0);
    ~ApplicationProject();

    QString fullPath() const;
    void setFullPath(const QString& path);

    Model::CalliperProject* project();
    const Model::CalliperProject* project() const;

    ModelLoaders::FileDataModelStore& fileStore();
    const ModelLoaders::FileDataModelStore& fileStore() const;

private:
    typedef QSharedPointer<Model::BaseFileDataModel> DataModelPointer;

    QString m_strFullPath;
    Model::CalliperProject* m_pProject;
    ModelLoaders::FileDataModelStore m_FileStore;
};

#endif // APPLICATIONPROJECT_H
