#include "projectfiletreewidget.h"
#include <QFileIconProvider>

namespace
{
    const char* fileTypeLabels[] =
    {
        "Other",

        "Maps",
    };
}

namespace AppCalliper
{
    ProjectFileTreeWidget::ProjectFileTreeWidget(QWidget *parent)
        : QTreeWidget(parent)
    {
        m_pProjectItem = new QTreeWidgetItem();
        m_pProjectItem->setData(0, Qt::DecorationRole, QFileIconProvider().icon(QFileIconProvider::File));

        invisibleRootItem()->addChild(m_pProjectItem);
    }

    QTreeWidgetItem* ProjectFileTreeWidget::rootProjectItem() const
    {
        return m_pProjectItem;
    }

    void ProjectFileTreeWidget::setRoot(const QString &projectFilePath)
    {
        QFileInfo fileInfo(projectFilePath);

        m_pProjectItem->setText(0, fileInfo.fileName());
        m_pProjectItem->setData(0, FilePathRole, fileInfo.canonicalFilePath());
    }

    void ProjectFileTreeWidget::addFile(FileType type, const QString &localPath)
    {
        if ( m_ItemsByPath.contains(localPath) )
        {
            return;
        }

        QTreeWidgetItem* item = m_ItemsByType.value(type, nullptr);
        if ( item == nullptr )
        {
            item = createFileTypeItem(type);
        }

        QFileIconProvider icons;

        QTreeWidgetItem* child = new QTreeWidgetItem();
        child->setData(0, Qt::DecorationRole, icons.icon(QFileIconProvider::File));
        child->setData(0, FilePathRole, localPath);
        child->setText(0, QFileInfo(localPath).fileName());

        item->addChild(child);
        m_ItemsByPath.insert(localPath, child);

        emit fileAdded(localPath);
    }

    void ProjectFileTreeWidget::removeFile(const QString &localPath)
    {
        QTreeWidgetItem* item = m_ItemsByPath.value(localPath, nullptr);
        if ( !item )
        {
            return;
        }

        QTreeWidgetItem* parent = item->parent();
        parent->removeChild(item);
        delete item;

        m_ItemsByPath.remove(localPath);

        if ( parent->childCount() < 1 )
        {
            FileType type = static_cast<FileType>(parent->data(0, FileTypeRole).toInt());
            parent->parent()->removeChild(parent);
            delete parent;
            m_ItemsByType.remove(type);
        }

        emit fileRemoved(localPath);
    }

    void ProjectFileTreeWidget::clearFiles()
    {
        while ( m_pProjectItem->childCount() > 0 )
        {
            QTreeWidgetItem* child = m_pProjectItem->child(0);
            m_pProjectItem->removeChild(child);
            delete child;
        }

        m_ItemsByPath.clear();
        m_ItemsByType.clear();
        emit filesCleared();
    }

    QTreeWidgetItem* ProjectFileTreeWidget::createFileTypeItem(FileType type)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem();

        item->setText(0, tr(fileTypeLabels[type]));
        item->setData(0, FileTypeRole, type);
        m_ItemsByType.insert(type, item);

        m_pProjectItem->addChild(item);
        return item;
    }
}
