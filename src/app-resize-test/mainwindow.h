#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "arrangeable-tabs/resizeablegridlayoutmanager.h"

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
    void addNewItem();

private:
    Ui::MainWindow *ui;
    UserInterface::ResizeableGridLayoutManager* m_pGridManager;
};

#endif // MAINWINDOW_H
