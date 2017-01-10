#ifndef CALLIPERPROJECTLOADER_H
#define CALLIPERPROJECTLOADER_H

#include "model-loaders_global.h"
#include "projects/calliperproject.h"
#include <QJsonDocument>

namespace ModelLoaders
{
    class MODELLOADERSSHARED_EXPORT CalliperProjectLoader
    {
    public:
        explicit CalliperProjectLoader(Model::CalliperProject* project);

        Model::CalliperProject* project() const;
        void setProject(Model::CalliperProject* project);

        QJsonDocument toJsonDocument() const;
        void fromJsonDocument(const QJsonDocument& doc);

    private:
        void populateMetadata(const QJsonObject& json);

        QJsonObject exportMetadata() const;

        Model::CalliperProject* m_pProject;
    };
}

#endif // CALLIPERPROJECTLOADER_H
