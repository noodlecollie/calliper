#include "calliperprojectmetadata.h"
#include <QSignalBlocker>
#include <QMetaObject>
#include <QtDebug>

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
        setVersion(0);

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

    int CalliperProjectMetadata::version() const
    {
        return m_iVersion;
    }

    void CalliperProjectMetadata::setVersion(int version)
    {
        if ( version == m_iVersion )
            return;

        m_iVersion = version;
        notifyDataChanged(metaObject()->indexOfProperty("version"));
    }
}
