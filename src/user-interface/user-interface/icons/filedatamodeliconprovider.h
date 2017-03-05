#ifndef FILEDATAMODELICONPROVIDER_H
#define FILEDATAMODELICONPROVIDER_H

#include "user-interface_global.h"
#include "model/filedatamodels/base/basefiledatamodel.h"
#include <QIcon>

namespace UserInterface
{
    class USERINTERFACESHARED_EXPORT FileDataModelIconProvider
    {
    public:
        FileDataModelIconProvider();

        QIcon calliperProjectIcon() const;

        bool hasIcon(Model::BaseFileDataModel::ModelType modelType) const;
        QIcon icon(Model::BaseFileDataModel::ModelType modelType) const;

    private:
        void addIcon(Model::BaseFileDataModel::ModelType modelType);

        static bool m_bInitialised;
        static QHash<Model::BaseFileDataModel::ModelType, QString> m_IconTable;
    };
}

#endif // FILEDATAMODELICONPROVIDER_H
