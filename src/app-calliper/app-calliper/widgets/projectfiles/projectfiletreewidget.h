#ifndef PROJECTFILETREEWIDGET_H
#define PROJECTFILETREEWIDGET_H

#include <QTreeWidget>
#include <QHash>
#include "model/filedatamodels/base/basefiledatamodel.h"

class QTreeWidgetItem;

class ProjectFileTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    enum ItemUserRole
    {
        FilePathRole = Qt::UserRole,
        FileTypeRole,
        ItemFlagsRole,
    };

    enum ItemFlag
    {
        NoItemFlag = 0,

        ProjectItemFlag = 0x1,
    };
    Q_DECLARE_FLAGS(ItemFlags, ItemFlag)
    Q_FLAG(ItemFlags)

    explicit ProjectFileTreeWidget(QWidget *parent = 0);

    QTreeWidgetItem* rootProjectItem() const;
    void setProject(const QString& projectFilePath);
    bool hasProject() const;

    void addFile(Model::BaseFileDataModel::ModelType type, const QString& localPath, bool shouldExpandItem = false);
    void removeFile(const QString& localPath);
    void clear();
    void clearProjectFiles();

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
    void expandAncestorPath(QTreeWidgetItem* item);

    QTreeWidgetItem* m_pProjectItem;
    bool m_bHasProject;

    QHash<QString, QTreeWidgetItem*> m_ItemsByPath;
    QHash<Model::BaseFileDataModel::ModelType, QTreeWidgetItem*> m_ItemsByType;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ProjectFileTreeWidget::ItemFlags)

#endif // PROJECTFILETREEWIDGET_H
