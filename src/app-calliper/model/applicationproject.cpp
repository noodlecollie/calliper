#include "applicationproject.h"

namespace AppCalliper
{
    ApplicationProject::ApplicationProject(QObject *parent)
        : Model::DataChangeNotifier(parent),
          m_pProject(new Model::CalliperProject(this))
    {
        connectDataChangedSignals(m_pProject);
    }

    QString ApplicationProject::fileName() const
    {
        return m_strFileName;
    }

    void ApplicationProject::setFileName(const QString &filename)
    {
        m_strFileName = filename;
    }

    Model::CalliperProject* ApplicationProject::project()
    {
        return m_pProject;
    }

    const Model::CalliperProject* ApplicationProject::project() const
    {
        return m_pProject;
    }
}
