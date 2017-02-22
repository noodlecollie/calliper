#include "projectfiletreewidget.h"
#include <QFileIconProvider>
#include <QContextMenuEvent>
#include <QMenu>

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
        : QTreeWidget(parent),
          m_pProjectItem(nullptr),
          m_bHasProject(false)
    {
        createBlankProjectItem();
    }

    QTreeWidgetItem* ProjectFileTreeWidget::rootProjectItem() const
    {
        return m_pProjectItem;
    }

    void ProjectFileTreeWidget::setProject(const QString &projectFilePath)
    {
        QFileInfo fileInfo(projectFilePath);

        m_pProjectItem->setText(0, fileInfo.fileName());
        m_pProjectItem->setData(0, FilePathRole, fileInfo.canonicalFilePath());
        m_pProjectItem->setData(0, Qt::DecorationRole, QFileIconProvider().icon(QFileIconProvider::File));

        m_bHasProject = true;
    }

    bool ProjectFileTreeWidget::hasProject() const
    {
        return m_bHasProject;
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
        createBlankProjectItem();

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

    void ProjectFileTreeWidget::contextMenuEvent(QContextMenuEvent *event)
    {
        QMenu menu;

        createMenuAction(menu, tr("&Add new file..."), SIGNAL(addNewFileRequested()));

        menu.exec(event->globalPos());
    }

    // TODO: Allow passing lambdas. Can't see how Qt does this ATM as I'm not on the internet.
    void ProjectFileTreeWidget::createMenuAction(QMenu &menu, const QString &text, const char *sigOrSlot)
    {
        QAction* action = menu.addAction(text, this, sigOrSlot);
        action->setEnabled(hasProject());
    }

    void ProjectFileTreeWidget::createBlankProjectItem()
    {
        delete m_pProjectItem;
        m_pProjectItem = new QTreeWidgetItem();
        invisibleRootItem()->addChild(m_pProjectItem);

        m_bHasProject = false;
    }
}
