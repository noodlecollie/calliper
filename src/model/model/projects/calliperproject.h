#ifndef CALLIPERPROJECT_H
#define CALLIPERPROJECT_H

#include "model_global.h"
#include "model/core/datachangenotifier.h"
#include "calliperprojectmetadata.h"
#include <QSet>

namespace Model
{
    class MODELSHARED_EXPORT CalliperProject : public DataChangeNotifier
    {
        Q_OBJECT
    public:
        explicit CalliperProject(QObject* parent = 0);

        void clear();

        CalliperProjectMetadata* metadata();
        const CalliperProjectMetadata* metadata() const;

        void addProjectFile(const QString& localFilePath);
        void removeProjectFile(const QString& localFilePath);
        int projectFileCount() const;
        const QSet<QString>& projectFiles() const;

    private:
        CalliperProjectMetadata* m_pMetadata;
        QSet<QString> m_ProjectFiles;
    };
}

#endif // CALLIPERPROJECT_H
