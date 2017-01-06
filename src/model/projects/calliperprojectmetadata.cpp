#include "calliperprojectmetadata.h"
#include <QSignalBlocker>
#include <QMetaObject>

namespace Model
{
    CalliperProjectMetadata::CalliperProjectMetadata(QObject *parent)
        : DataChangeNotifier(parent)
    {
        clear();
    }

    void CalliperProjectMetadata::clear()
    {
        QSignalBlocker blocker(this);
        Q_UNUSED(blocker);

        setProjectName(QString());

        notifyDataChanged();
    }

    QString CalliperProjectMetadata::projectName() const
    {
        return m_strProjectName;
    }

    void CalliperProjectMetadata::setProjectName(const QString &name)
    {
        if ( name == m_strProjectName )
            return;

        m_strProjectName = name;
        notifyDataChanged(metaObject()->indexOfProperty("projectName"));
    }
}
