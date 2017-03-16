#include "applicationproject.h"
#include "model-loaders/filedataloaders/fileextensiondatamodelmap.h"
#include "model/filedatamodels/filedatamodelfactory.h"
#include "model-loaders/filedataloaders/filedataloaderfactory.h"
#include <QFileInfo>
#include <QScopedPointer>

namespace AppCalliper
{
    ApplicationProject::ApplicationProject(QObject *parent)
        : Model::DataChangeNotifier(parent),
          m_pProject(new Model::CalliperProject(this))
    {
        connectDataChangedSignals(m_pProject);
    }

    ApplicationProject::~ApplicationProject()
    {
    }

    QString ApplicationProject::fullPath() const
    {
        return m_strFullPath;
    }

    void ApplicationProject::setFullPath(const QString &path)
    {
        m_strFullPath = path;
    }

    Model::CalliperProject* ApplicationProject::project()
    {
        return m_pProject;
    }

    const Model::CalliperProject* ApplicationProject::project() const
    {
        return m_pProject;
    }

    ModelLoaders::FileDataModelStore& ApplicationProject::fileStore()
    {
        return m_FileStore;
    }

    const ModelLoaders::FileDataModelStore& ApplicationProject::fileStore() const
    {
        return m_FileStore;
    }
}
