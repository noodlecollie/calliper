#ifndef CALLIPERPROJECTMETADATA_H
#define CALLIPERPROJECTMETADATA_H

#include "model_global.h"
#include "core/datachangenotifier.h"
#include <QString>

namespace Model
{
    class CalliperProjectMetadata : public DataChangeNotifier
    {
        Q_OBJECT
        Q_PROPERTY(QString projectName READ projectName WRITE setProjectName)
    public:
        explicit CalliperProjectMetadata(QObject* parent);

        void clear();

        QString projectName() const;
        void setProjectName(const QString& name);

    private:
        QString m_strProjectName;
    };
}

#endif // CALLIPERPROJECTMETADATA_H
