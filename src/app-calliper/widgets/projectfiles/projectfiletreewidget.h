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
        void setRoot(const QString& projectFilePath);

        void addFile(FileType type, const QString& localPath);
        void removeFile(const QString& localPath);
        void clearFiles();

    signals:
        void fileAdded(const QString& localFilePath);
        void fileRemoved(const QString& localFilePath);
        void filesCleared();

    private:
        QTreeWidgetItem* createFileTypeItem(FileType type);

        QTreeWidgetItem* m_pProjectItem;
        QHash<QString, QTreeWidgetItem*> m_ItemsByPath;
        QHash<FileType, QTreeWidgetItem*> m_ItemsByType;
    };
}

#endif // PROJECTFILETREEWIDGET_H
