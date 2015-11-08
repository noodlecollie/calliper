#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class COpenGLRenderer;
class CScene;
class QOffscreenSurface;

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
    
private:
    void initOpenGL();

    Ui::MainWindow *ui;
    COpenGLRenderer*    m_pRenderer;
    QOffscreenSurface*  m_pSurface;
    bool                m_bRenderInitRun;
    CScene*             m_pScene;
};

MainWindow* appMainWindow();

#endif // MAINWINDOW_H
