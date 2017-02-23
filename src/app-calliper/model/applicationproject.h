#ifndef APPLICATIONPROJECT_H
#define APPLICATIONPROJECT_H

#include "app-calliper_global.h"
#include "model/projects/calliperproject.h"
#include "model/core/datachangenotifier.h"
#include <QHash>
#include "model/filedatamodels/base/basefiledatamodel.h"

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

        // Maps a local file path to a model for that file.
        QHash<QString, Model::BaseFileDataModel*> m_FileModels;
    };
}

#endif // APPLICATIONPROJECT_H
