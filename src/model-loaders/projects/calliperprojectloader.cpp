#include "calliperprojectloader.h"
#include <QJsonObject>

namespace ModelLoaders
{
    CalliperProjectLoader::CalliperProjectLoader(Model::CalliperProject *project)
        : m_pProject(project)
    {
    }

    Model::CalliperProject* CalliperProjectLoader::project() const
    {
        return m_pProject;
    }

    void CalliperProjectLoader::setProject(Model::CalliperProject *project)
    {
        m_pProject = project;
    }

    QJsonDocument CalliperProjectLoader::toJsonDocument() const
    {
        if ( !m_pProject )
            return QJsonDocument();

        QJsonObject root;
        root["metadata"]  = exportMetadata();

        QJsonDocument doc;
        doc.setObject(root);
        return doc;
    }

    void CalliperProjectLoader::fromJsonDocument(const QJsonDocument &doc)
    {
        if ( !m_pProject )
            return;

        if ( !doc.isObject() )
            return;

        QJsonObject root = doc.object();
        m_pProject->clear();

        populateMetadata(root["metadata"].toObject());
    }

    void CalliperProjectLoader::populateMetadata(const QJsonObject& json)
    {
        Model::CalliperProjectMetadata& metadata = m_pProject->metadata();

        QJsonValue vProjectName = json.value("projectName");
        if ( vProjectName.isString() )
        {
            metadata.setProjectName(vProjectName.toString());
        }
    }

    QJsonObject CalliperProjectLoader::exportMetadata() const
    {
        const Model::CalliperProjectMetadata& metadata = m_pProject->metadata();
        QJsonObject obj;

        obj["projectName"] = metadata.projectName();

        return obj;
    }
}
