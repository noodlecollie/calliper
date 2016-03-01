#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "mapdocument.h"
#include "callipermapfile.h"

namespace Ui {
class MainWindow;
}

class MapScene;
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
	void toolButtonClicked();
    void saveCurrentDocumentAs();
    void saveCurrentDocument();
    void loadDocument();

private:
    void setUpConnections();
    void populateSceneTree(MapScene* scene);
    void populateSceneTreeRecursive(SceneObject* object, QTreeWidgetItem* parent, QList<QTreeWidgetItem*> &items);
    void changeActiveDocument(MapDocument* oldDoc, MapDocument* newDoc);
    void saveDocument(MapDocument* document, const QString &filename, CalliperMapFile::FileFormat format);
    void loadDocument(MapDocument* document, const QString &filename);

    Ui::MainWindow *ui;

    int m_iActiveDocument;
    Viewport* m_pActiveViewport;
    QString m_szLastSaveDir;
    QString m_szLastLoadDir;
    CalliperMapFile::FileFormat m_iLastSaveFormat;
};

#endif // MAINWINDOW_H
