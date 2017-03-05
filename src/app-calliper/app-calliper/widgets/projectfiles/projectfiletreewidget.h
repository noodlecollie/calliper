#ifndef PROJECTFILETREEWIDGET_H
#define PROJECTFILETREEWIDGET_H

#include "app-calliper_global.h"
#include <QTreeWidget>
#include <QHash>
#include "model/filedatamodels/base/basefiledatamodel.h"

class QTreeWidgetItem;

namespace AppCalliper
{
    class ProjectFileTreeWidget : public QTreeWidget
    {
        Q_OBJECT
    public:
        enum ItemUserRole
        {
            FilePathRole = Qt::UserRole,
            FileTypeRole,
        };

        explicit ProjectFileTreeWidget(QWidget *parent = 0);

        QTreeWidgetItem* rootProjectItem() const;
        void setProject(const QString& projectFilePath);
        bool hasProject() const;

        void addFile(Model::BaseFileDataModel::ModelType type, const QString& localPath, bool shouldExpandItem = false);
        void removeFile(const QString& localPath);
        void clearFiles();

    signals:
        void fileAdded(const QString& localFilePath);
        void fileRemoved(const QString& localFilePath);
        void filesCleared();

        void addNewFileRequested();

    protected:
        void contextMenuEvent(QContextMenuEvent *event);

    private:
        QTreeWidgetItem* createFileTypeItem(Model::BaseFileDataModel::ModelType type);
        void createBlankProjectItem();
        void createMenuAction(QMenu& menu, const QString& text, const char* sigOrSlot);

        QTreeWidgetItem* m_pProjectItem;
        bool m_bHasProject;

        QHash<QString, QTreeWidgetItem*> m_ItemsByPath;
        QHash<Model::BaseFileDataModel::ModelType, QTreeWidgetItem*> m_ItemsByType;
    };
}

#endif // PROJECTFILETREEWIDGET_H
