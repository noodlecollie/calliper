#ifndef CALLIPERPROJECTMETADATA_H
#define CALLIPERPROJECTMETADATA_H

#include "model_global.h"
#include "core/datachangenotifier.h"
#include <QString>

namespace Model
{
    class MODELSHARED_EXPORT CalliperProjectMetadata : public DataChangeNotifier
    {
        Q_OBJECT

        // No QProperty write as the version shouldn't be set by observers,
        // only by the file import.
        Q_PROPERTY(int version READ version)

        Q_PROPERTY(QString projectName READ projectName WRITE setProjectName)
    public:
        explicit CalliperProjectMetadata(QObject* parent);

        void clear();

        QString projectName() const;
        void setProjectName(const QString& name);

        int version() const;
        void setVersion(int version);

    private:
        QString m_strProjectName;
        int m_iVersion;
    };
}

#endif // CALLIPERPROJECTMETADATA_H
