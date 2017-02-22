#include "calliperproject.h"

namespace Model
{
    CalliperProject::CalliperProject(QObject* parent)
        : DataChangeNotifier(parent),
          m_pMetadata(new CalliperProjectMetadata(this))
    {
        connectDataChangedSignals(m_pMetadata);
    }

    void CalliperProject::clear()
    {
        m_pMetadata->clear();
        m_ProjectFiles.clear();
    }

    CalliperProjectMetadata* CalliperProject::metadata()
    {
        return m_pMetadata;
    }

    const CalliperProjectMetadata* CalliperProject::metadata() const
    {
        return m_pMetadata;
    }

    void CalliperProject::addProjectFile(const QString &localFilePath)
    {
        m_ProjectFiles.insert(localFilePath);
    }

    void CalliperProject::removeProjectFile(const QString &localFilePath)
    {
        m_ProjectFiles.remove(localFilePath);
    }

    int CalliperProject::projectFileCount() const
    {
        return m_ProjectFiles.count();
    }

    const QSet<QString>& CalliperProject::projectFiles() const
    {
        return m_ProjectFiles;
    }
}
