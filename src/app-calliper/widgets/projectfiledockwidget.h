#ifndef PROJECTFILEDOCKWIDGET_H
#define PROJECTFILEDOCKWIDGET_H

#include "app-calliper_global.h"
#include "visibleactiondockwidget.h"

class QTreeWidget;
class QTreeWidgetItem;

namespace AppCalliper
{
    class ProjectFileDockWidget : public VisibleActionDockWidget
    {
        Q_OBJECT
    public:
        ProjectFileDockWidget(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

        void setRoot(const QString& projectFileName);

        void addFile(const QString& localPath);
        void removeFile(const QString& localPath);
        void clearFiles();

    signals:
        void fileAdded(const QString& localFilePath);
        void fileRemoved(const QString& localFilePath);
        void fileDoubleClicked(const QString& localFilePath);
        void filesCleared();

    private slots:
        void itemDoubleClicked(QTreeWidgetItem* item, int column);

    private:
        void createEntry(const QStringList& filePathSections);
        void removeEntry(const QStringList& filePathSections);
        bool removeEntryRecursive(QTreeWidgetItem* parent, const QStringList& filePathSections, int index);
        QTreeWidgetItem* getRootItem() const;

        QTreeWidget* m_pTreeWidget;
    };
}

#endif // PROJECTFILEDOCKWIDGET_H
