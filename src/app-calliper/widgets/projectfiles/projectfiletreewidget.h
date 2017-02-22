#ifndef PROJECTFILETREEWIDGET_H
#define PROJECTFILETREEWIDGET_H

#include "app-calliper_global.h"
#include <QTreeWidget>
#include <QHash>

class QTreeWidgetItem;

namespace AppCalliper
{
    class ProjectFileTreeWidget : public QTreeWidget
    {
        Q_OBJECT
    public:
        enum FileType
        {
            UnknownFile = 0,

            MapFile,
        };

        enum ItemUserRole
        {
            FilePathRole = Qt::UserRole,
            FileTypeRole,
        };

        explicit ProjectFileTreeWidget(QWidget *parent = 0);

        QTreeWidgetItem* rootProjectItem() const;
        void setProject(const QString& projectFilePath);
        bool hasProject() const;

        void addFile(FileType type, const QString& localPath);
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
        QTreeWidgetItem* createFileTypeItem(FileType type);
        void createBlankProjectItem();
        void createMenuAction(QMenu& menu, const QString& text, const char* sigOrSlot);

        QTreeWidgetItem* m_pProjectItem;
        bool m_bHasProject;

        QHash<QString, QTreeWidgetItem*> m_ItemsByPath;
        QHash<FileType, QTreeWidgetItem*> m_ItemsByType;
    };
}

#endif // PROJECTFILETREEWIDGET_H
