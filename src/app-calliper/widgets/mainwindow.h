#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "projects/calliperproject.h"
#include "projects/calliperproject.h"

class QTreeWidget;
class ProjectFileDockWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void menuOpenProject();
    void menuSaveCurrentProject();
    void menuSaveCurrentProjectAs();

private:
    void initDockWidgets();
    void saveCurrentProject(const QString& filename);
    QString getFileDialogueDefaultPath() const;
    void updateWindowTitle();
    void repopulateProjectFileTree();
    bool hasValidProject() const;

    Ui::MainWindow *ui;

    ProjectFileDockWidget* m_pProjectFileDockWidget;
    QString m_strProjectFileName;
    Model::CalliperProject m_Project;
};

#endif // MAINWINDOW_H
