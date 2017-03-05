#include "projectfiletreewidget.h"
#include <QFileIconProvider>
#include <QContextMenuEvent>
#include <QMenu>
#include <QtDebug>
#include <QCoreApplication>

namespace
{
    QString translatedTypeLabel(Model::BaseFileDataModel::ModelType type)
    {
        static QHash<Model::BaseFileDataModel::ModelType, QString> names;

        if ( names.isEmpty() )
        {
            names.insert(Model::BaseFileDataModel::MapModel, "Maps");
        }

        return QCoreApplication::translate(AppCalliper::ProjectFileTreeWidget::staticMetaObject.className(),
                                           qPrintable(names.value(type, "Other")));
    }
}

namespace AppCalliper
{
    ProjectFileTreeWidget::ProjectFileTreeWidget(QWidget *parent)
        : QTreeWidget(parent),
          m_pProjectItem(Q_NULLPTR),
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

    void ProjectFileTreeWidget::addFile(Model::BaseFileDataModel::ModelType type, const QString &localPath, bool shouldExpandItem)
    {
        if ( m_ItemsByPath.contains(localPath) )
        {
            return;
        }

        QTreeWidgetItem* item = m_ItemsByType.value(type, Q_NULLPTR);
        if ( item == Q_NULLPTR )
        {
            item = createFileTypeItem(type);
        }

        QFileIconProvider icons;

        QTreeWidgetItem* child = new QTreeWidgetItem();
        child->setData(0, Qt::DecorationRole, icons.icon(QFileIconProvider::File));
        child->setData(0, FilePathRole, localPath);
        child->setText(0, QFileInfo(localPath).fileName());
        qDebug() << "Local path:" << localPath;

        item->addChild(child);
        m_ItemsByPath.insert(localPath, child);

        // TODO: Fix
        if ( shouldExpandItem )
        {
            expandItem(item);
        }

        emit fileAdded(localPath);
    }

    void ProjectFileTreeWidget::removeFile(const QString &localPath)
    {
        QTreeWidgetItem* item = m_ItemsByPath.value(localPath, Q_NULLPTR);
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
            Model::BaseFileDataModel::ModelType type = static_cast<Model::BaseFileDataModel::ModelType>(parent->data(0, FileTypeRole).toInt());
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

    QTreeWidgetItem* ProjectFileTreeWidget::createFileTypeItem(Model::BaseFileDataModel::ModelType type)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem();

        item->setText(0, translatedTypeLabel(type));
        item->setData(0, FileTypeRole, type);
        m_ItemsByType.insert(type, item);

        m_pProjectItem->addChild(item);
        return item;
    }

    void ProjectFileTreeWidget::contextMenuEvent(QContextMenuEvent *event)
    {
        QTreeWidgetItem* item = itemAt(event->pos());
        if ( item )
        {
            // Handle custom menus for items here at some point.
            return;
        }

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
        m_pProjectItem->setFlags(m_pProjectItem->flags() & ~Qt::ItemIsSelectable);
        invisibleRootItem()->addChild(m_pProjectItem);

        m_bHasProject = false;
    }
}
