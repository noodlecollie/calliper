#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "mapdocument.h"

namespace Ui {
class MainWindow;
}

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

private:
    void updateFromActiveDocument();
    MapDocument* activeDocument() const;

    Ui::MainWindow *ui;

    int m_iActiveDocument;
};

MainWindow* appMainWindow();

#endif // MAINWINDOW_H
