#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "user-interface/arrangeable-tabs/resizeablegridlayoutmanager.h"

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
    void removeItem();
    void equalise();
    void widgetFloated(QWidget* widget, bool dragged);

private:
    void setWidgetInGrid(QWidget* widget);

    Ui::MainWindow *ui;
    UserInterface::ResizeableGridLayoutManager* m_pGridManager;
};

#endif // MAINWINDOW_H
