#ifndef APPLICATIONPROJECT_H
#define APPLICATIONPROJECT_H

#include "app-calliper_global.h"
#include "projects/calliperproject.h"
#include "core/datachangenotifier.h"

namespace AppCalliper
{
    class ApplicationProject : public Model::DataChangeNotifier
    {
        Q_OBJECT
    public:
        explicit ApplicationProject(QObject* parent = 0);

        QString fileName() const;
        void setFileName(const QString& filename);

        Model::CalliperProject* project();
        const Model::CalliperProject* project() const;

    private:
        QString m_strFileName;
        Model::CalliperProject* m_pProject;
    };
}

#endif // APPLICATIONPROJECT_H
