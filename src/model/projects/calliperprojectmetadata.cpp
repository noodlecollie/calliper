#include "calliperprojectmetadata.h"

namespace Model
{
    CalliperProjectMetadata::CalliperProjectMetadata()
    {
        clear();
    }

    void CalliperProjectMetadata::clear()
    {
        m_strProjectName = QString();
    }

    QString CalliperProjectMetadata::projectName() const
    {
        return m_strProjectName;
    }

    void CalliperProjectMetadata::setProjectName(const QString &name)
    {
        m_strProjectName = name;
    }
}
