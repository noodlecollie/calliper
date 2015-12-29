#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "mapdocument.h"

namespace Ui {
class MainWindow;
}

class Scene;
class SceneObject;
class QTreeWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void quit();
    void updateDocumentList(const QList<MapDocument*> &docs);
    void createNewDocument();
    void makeDocumentActiveFromMenu();
    void closeActiveDocument();
    void changeDockWidgetVisibility(bool visible);

private:
    void updateFromActiveDocument();
    MapDocument* activeDocument() const;
    void setUpConnections();
    void populateSceneTree(Scene* scene);
    void populateSceneTreeRecursive(SceneObject* object, QTreeWidgetItem* parent, QList<QTreeWidgetItem*> &items);

    Ui::MainWindow *ui;

    int m_iActiveDocument;
};

MainWindow* appMainWindow();

#endif // MAINWINDOW_H
