#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class COpenGLRenderer;
class CScene;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    COpenGLRenderer* renderer() const;
    CScene* scene() const;

protected:
    virtual void showEvent(QShowEvent *e);
    
private:
    Ui::MainWindow *ui;
    COpenGLRenderer*    m_pRenderer;
    CScene*             m_pScene;
};

MainWindow* appMainWindow();

#endif // MAINWINDOW_H
