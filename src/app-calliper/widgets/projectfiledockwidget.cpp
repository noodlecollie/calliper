#include "projectfiledockwidget.h"
#include <QTreeWidget>
#include <QDir>
#include <QFileIconProvider>
#include <QtDebug>
#include <QApplication>

namespace
{
    enum TreeItemUserRole
    {
        FilePathRole = Qt::UserRole,
    };

    const char* fileTypeLabels[] =
    {
        "Other",

        "Maps",
    };
}

namespace AppCalliper
{
    ProjectFileDockWidget::ProjectFileDockWidget(QWidget *parent, Qt::WindowFlags flags)
        : VisibleActionDockWidget(tr("Project Files"), parent, flags)
    {
        m_pTreeWidget = new QTreeWidget();
        m_pTreeWidget->setHeaderLabel(tr("Files"));
        m_pTreeWidget->setAnimated(true);

        setWidget(m_pTreeWidget);

        connect(m_pTreeWidget, &QTreeWidget::itemDoubleClicked, this, &ProjectFileDockWidget::itemDoubleClicked);
    }

    void ProjectFileDockWidget::addFile(FileType type, const QString &localPath)
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

    void ProjectFileDockWidget::removeFile(const QString &localPath)
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

        emit fileRemoved(localPath);
    }

    void ProjectFileDockWidget::clearFiles()
    {
        m_pTreeWidget->clear();
        m_ItemsByPath.clear();
        m_ItemsByType.clear();
        emit filesCleared();
    }

    void ProjectFileDockWidget::itemDoubleClicked(QTreeWidgetItem *item, int column)
    {
        Q_UNUSED(column);

        QString filePath = item->data(0, FilePathRole).toString();

        if ( !filePath.isNull() )
            emit fileDoubleClicked(filePath);
    }

    QTreeWidgetItem* ProjectFileDockWidget::getRootItem() const
    {
        QTreeWidgetItem* root = m_pTreeWidget->invisibleRootItem();
        if ( root->childCount() < 1 )
            return root;

        return root->child(0);
    }

    void ProjectFileDockWidget::setRoot(const QString &projectFilePath)
    {
        QTreeWidgetItem* root = getRootItem();
        if ( root == m_pTreeWidget->invisibleRootItem() )
        {
            QTreeWidgetItem* item = new QTreeWidgetItem();
            root->addChild(item);
            root = item;
        }

        QFileInfo fileInfo(projectFilePath);

        root->setText(0, fileInfo.fileName());
        root->setData(0, FilePathRole, fileInfo.canonicalFilePath());
        root->setData(0, Qt::DecorationRole, QFileIconProvider().icon(QFileIconProvider::File));
    }

    QTreeWidgetItem* ProjectFileDockWidget::createFileTypeItem(FileType type)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem();

        item->setText(0, tr(fileTypeLabels[type]));
        m_ItemsByType.insert(type, item);

        getRootItem()->addChild(item);
        return item;
    }
}
