#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class COpenGLRenderer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void showEvent(QShowEvent *e);
    
private:
    Ui::MainWindow *ui;
    COpenGLRenderer*    m_pRenderer;
};

#endif // MAINWINDOW_H
