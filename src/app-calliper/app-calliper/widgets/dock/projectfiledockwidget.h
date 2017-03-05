#ifndef PROJECTFILEDOCKWIDGET_H
#define PROJECTFILEDOCKWIDGET_H

#include "app-calliper_global.h"
#include "app-calliper/widgets/dock/visibleactiondockwidget.h"
#include "app-calliper/widgets/projectfiles/projectfiletreewidget.h"

namespace AppCalliper
{
    class ProjectFileDockWidget : public VisibleActionDockWidget
    {
        Q_OBJECT
    public:
        ProjectFileDockWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());

        void addFile(Model::BaseFileDataModel::ModelType type, const QString& localPath);
        void removeFile(const QString& localPath);
        void clearFiles();
        void setProject(const QString& projectFilePath);

    signals:
        void fileAdded(const QString& localFilePath);
        void fileRemoved(const QString& localFilePath);
        void fileDoubleClicked(const QString& localFilePath);
        void filesCleared();

        void addNewFileRequested();

    private slots:
        void itemDoubleClicked(QTreeWidgetItem* item, int column);

    private:
        ProjectFileTreeWidget* m_pTreeWidget;
    };
}

#endif // PROJECTFILEDOCKWIDGET_H
