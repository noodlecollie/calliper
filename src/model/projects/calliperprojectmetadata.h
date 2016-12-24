#ifndef CALLIPERPROJECTMETADATA_H
#define CALLIPERPROJECTMETADATA_H

#include "model_global.h"
#include <QString>

namespace Model
{
    class CalliperProjectMetadata
    {
    public:
        CalliperProjectMetadata();

        void clear();

        QString projectName() const;
        void setProjectName(const QString& name);

    private:
        QString m_strProjectName;
    };
}

#endif // CALLIPERPROJECTMETADATA_H
