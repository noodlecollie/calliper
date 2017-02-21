#include "projectfiledockwidget.h"
#include <QTreeWidget>
#include <QDir>
#include <QFileIconProvider>
#include <QtDebug>

namespace
{
    enum TreeItemUserRole
    {
        FilePathRole = Qt::UserRole,
    };

    QTreeWidgetItem* findChildItem(QTreeWidgetItem* item, const QString& text)
    {
        for ( int i = 0; i < item->childCount(); i++ )
        {
            QTreeWidgetItem* child = item->child(i);
            if ( child->text(0) == text )
                return child;
        }

        return nullptr;
    }

    int findChildIndex(QTreeWidgetItem* item, const QString& text)
    {
        for ( int i = 0; i < item->childCount(); i++ )
        {
            QTreeWidgetItem* child = item->child(i);
            if ( child->text(0) == text )
                return i;
        }

        return -1;
    }

    QString fileNameWithExtension(const QString& path)
    {
        return QFileInfo(path).fileName();
    }
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

    void ProjectFileDockWidget::addFile(const QString &localPath)
    {
        QStringList filePathSections = QDir::fromNativeSeparators(localPath).split("/");
        if ( filePathSections.isEmpty() )
            return;

        createEntry(filePathSections);
    }

    void ProjectFileDockWidget::removeFile(const QString &localPath)
    {
        QStringList filePathSections = QDir::fromNativeSeparators(localPath).split("/");
        if ( filePathSections.isEmpty() )
            return;

        removeEntry(filePathSections);
    }

    void ProjectFileDockWidget::clearFiles()
    {
        m_pTreeWidget->clear();
        emit filesCleared();
    }

    void ProjectFileDockWidget::createEntry(const QStringList &filePathSections)
    {
        QFileIconProvider icons;
        QTreeWidgetItem* item = getRootItem();

        for ( int i = 0; i < filePathSections.count(); i++ )
        {
            QTreeWidgetItem* child = findChildItem(item, filePathSections.at(i));
            if ( !child )
            {
                child = new QTreeWidgetItem(QStringList() << filePathSections.at(i));

                QFileIconProvider::IconType iconType = QFileIconProvider::Folder;

                if ( i == filePathSections.count() - 1 )
                {
                    iconType = QFileIconProvider::File;
                }

                child->setData(0, Qt::DecorationRole, icons.icon(iconType));
                item->addChild(child);
            }

            item = child;
        }

        QString path = filePathSections.join('/');
        item->setData(0, FilePathRole, path);
        emit fileAdded(path);
    }

    void ProjectFileDockWidget::removeEntry(const QStringList &filePathSections)
    {
        removeEntryRecursive(getRootItem(), filePathSections, 0);
    }

    bool ProjectFileDockWidget::removeEntryRecursive(QTreeWidgetItem *parent, const QStringList &filePathSections, int index)
    {
        if ( index < 0 || index >= filePathSections.count() )
            return false;

        int childIndex = findChildIndex(parent, filePathSections.at(index));
        if ( childIndex < 0 )
            return false;

        QTreeWidgetItem* child = parent->child(childIndex);
        bool canRemoveIntermediateItems = removeEntryRecursive(child, filePathSections, index + 1);

        bool childIsTarget = index == filePathSections.count() - 1 &&
                             child->data(0, FilePathRole).canConvert(QVariant::String);
        if ( childIsTarget )
        {
            // We found the target file we were removing!
            canRemoveIntermediateItems = true;
        }

        if ( childIsTarget || (canRemoveIntermediateItems && child->childCount() < 1) )
        {
            QString path = child->data(0, FilePathRole).toString();

            parent->removeChild(child);

            if ( childIsTarget )
                emit fileRemoved(path);
        }

        return canRemoveIntermediateItems;
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

        root->setText(0, fileNameWithExtension(fileInfo.fileName()));
        root->setData(0, FilePathRole, fileInfo.canonicalFilePath());
        root->setData(0, Qt::DecorationRole, QFileIconProvider().icon(QFileIconProvider::File));
    }
}
