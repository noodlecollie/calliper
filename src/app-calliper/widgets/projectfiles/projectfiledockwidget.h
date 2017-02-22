#ifndef PROJECTFILEDOCKWIDGET_H
#define PROJECTFILEDOCKWIDGET_H

#include "app-calliper_global.h"
#include "widgets/visibleactiondockwidget.h"
#include "projectfiletreewidget.h"

namespace AppCalliper
{
    class ProjectFileDockWidget : public VisibleActionDockWidget
    {
        Q_OBJECT
    public:
        ProjectFileDockWidget(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

        void clearFiles();
        void setRoot(const QString& projectFilePath);

    signals:
        void fileAdded(const QString& localFilePath);
        void fileRemoved(const QString& localFilePath);
        void fileDoubleClicked(const QString& localFilePath);
        void filesCleared();

    private slots:
        void itemDoubleClicked(QTreeWidgetItem* item, int column);

    private:
        ProjectFileTreeWidget* m_pTreeWidget;
    };
}

#endif // PROJECTFILEDOCKWIDGET_H
