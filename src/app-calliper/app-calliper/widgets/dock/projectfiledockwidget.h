#ifndef PROJECTFILEDOCKWIDGET_H
#define PROJECTFILEDOCKWIDGET_H

#include "app-calliper_global.h"
#include "app-calliper/widgets/projectfiles/projectfiletreewidget.h"

#include "user-interface/widgets/visibility-action/visibilityactiondockwidget.h"

namespace AppCalliper
{
    class ProjectFileDockWidget : public UserInterface::VisibilityActionDockWidget
    {
        Q_OBJECT
    public:
        ProjectFileDockWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());

        void addFile(Model::BaseFileDataModel::ModelType type, const QString& localPath, bool expandItem = false);
        void removeFile(const QString& localPath);
        void clear();
        void clearProjectFiles();
        void setProject(const QString& projectFilePath);
        void expandProjectItem();

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
