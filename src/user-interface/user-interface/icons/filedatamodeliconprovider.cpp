#include "filedatamodeliconprovider.h"
#include "calliperutil/qobject/qobjectutil.h"
#include <QtDebug>
#include <QMetaEnum>

namespace
{
    inline QString modelEnumName(Model::BaseFileDataModel::ModelType modelType)
    {
        return CalliperUtil::QObjectUtil::enumKeyFromValue<Model::BaseFileDataModel>("ModelType", modelType);
    }

    inline QString iconPath(const QString &iconName)
    {
        return QString(":/user-interface/icons/datamodel/%1").arg(iconName);
    }
}

namespace UserInterface
{
    bool FileDataModelIconProvider::m_bInitialised = false;
    QHash<Model::BaseFileDataModel::ModelType, QString> FileDataModelIconProvider::m_IconTable;

    FileDataModelIconProvider::FileDataModelIconProvider()
    {
        if ( m_bInitialised )
        {
            return;
        }

        addIcon(Model::BaseFileDataModel::MapModel);

        m_bInitialised = true;
    }

    QIcon FileDataModelIconProvider::calliperProjectIcon() const
    {
        return QIcon(":/user-interface/icons/project/calliperproject");
    }

    bool FileDataModelIconProvider::hasIcon(Model::BaseFileDataModel::ModelType modelType) const
    {
        return m_IconTable.contains(modelType);
    }

    QIcon FileDataModelIconProvider::icon(Model::BaseFileDataModel::ModelType modelType) const
    {
        if ( !hasIcon(modelType) )
        {
            return QIcon();
        }

        return QIcon(m_IconTable[modelType]);
    }

    void FileDataModelIconProvider::addIcon(Model::BaseFileDataModel::ModelType modelType)
    {
        Q_ASSERT_X(!m_IconTable.contains(modelType), Q_FUNC_INFO, "Icon entry already exists for this model type!");

        m_IconTable.insert(modelType, iconPath(modelEnumName(modelType)));
    }
}
