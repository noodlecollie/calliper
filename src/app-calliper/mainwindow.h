#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>

class QTreeWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initDockWidgets();

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
