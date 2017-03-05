#include "projectfiledockwidget.h"
#include <QTreeWidget>
#include <QDir>
#include <QFileIconProvider>
#include <QtDebug>
#include <QApplication>
#include <QContextMenuEvent>

namespace AppCalliper
{
    ProjectFileDockWidget::ProjectFileDockWidget(QWidget *parent, Qt::WindowFlags flags)
        : VisibleActionDockWidget(tr("Project Files"), parent, flags)
    {
        m_pTreeWidget = new ProjectFileTreeWidget();
        m_pTreeWidget->setHeaderLabel(tr("Files"));
        m_pTreeWidget->setAnimated(true);

        setWidget(m_pTreeWidget);

        connect(m_pTreeWidget, &QTreeWidget::itemDoubleClicked, this, &ProjectFileDockWidget::itemDoubleClicked);
        connect(m_pTreeWidget, &ProjectFileTreeWidget::fileAdded, this, &ProjectFileDockWidget::fileAdded);
        connect(m_pTreeWidget, &ProjectFileTreeWidget::fileRemoved, this, &ProjectFileDockWidget::fileRemoved);
        connect(m_pTreeWidget, &ProjectFileTreeWidget::filesCleared, this, &ProjectFileDockWidget::filesCleared);

        connect(m_pTreeWidget, &ProjectFileTreeWidget::addNewFileRequested, this, &ProjectFileDockWidget::addNewFileRequested);
    }

    void ProjectFileDockWidget::itemDoubleClicked(QTreeWidgetItem *item, int column)
    {
        Q_UNUSED(column);

        if ( !item->flags().testFlag(Qt::ItemIsSelectable) )
        {
            return;
        }

        QString filePath = item->data(0, ProjectFileTreeWidget::FilePathRole).toString();

        if ( filePath.isNull() )
        {
            return;
        }

        emit fileDoubleClicked(filePath);
    }

    void ProjectFileDockWidget::addFile(Model::BaseFileDataModel::ModelType type, const QString &localPath)
    {
        m_pTreeWidget->addFile(type, localPath);
    }

    void ProjectFileDockWidget::removeFile(const QString &localPath)
    {
        m_pTreeWidget->removeFile(localPath);
    }

    void ProjectFileDockWidget::clearFiles()
    {
        m_pTreeWidget->clearFiles();
    }

    void ProjectFileDockWidget::setProject(const QString &projectFilePath)
    {
        m_pTreeWidget->setProject(projectFilePath);
    }
}
