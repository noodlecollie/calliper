#include "mainwindow.h"

MainWindow::MainWindow() :
    QOpenGLWindow(), m_pScene(NULL), m_pSceneRenderer(NULL)
{

}

MainWindow::~MainWindow()
{
    delete m_pSceneRenderer;
    m_pSceneRenderer = NULL;

    delete m_pScene;
    m_pScene = NULL;
}


void MainWindow::initializeGL()
{

}

void MainWindow::paintGL()
{

}

void MainWindow::resizeGL(int w, int h)
{

}
