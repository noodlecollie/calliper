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
class Viewport;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class Viewport;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    MapDocument* activeDocument() const;
    Viewport* activeViewport() const;

public slots:
    void quit();
    void updateDocumentList(const QList<MapDocument*> &docs);
    void createNewDocument();
    void makeDocumentActiveFromMenu();
    void closeActiveDocument();
    void changeDockWidgetVisibility(bool visible);
    void sceneTreeItemDoubleClicked(QTreeWidgetItem* item, int column);
    void sceneTreeItemClicked(QTreeWidgetItem* item, int column);
    void viewportPreFrame(int msec);

private:
    void setUpConnections();
    void populateSceneTree(Scene* scene);
    void populateSceneTreeRecursive(SceneObject* object, QTreeWidgetItem* parent, QList<QTreeWidgetItem*> &items);
    void changeActiveDocument(MapDocument* oldDoc, MapDocument* newDoc);

    Ui::MainWindow *ui;

    int m_iActiveDocument;
    Viewport* m_pActiveViewport;
};

#endif // MAINWINDOW_H
