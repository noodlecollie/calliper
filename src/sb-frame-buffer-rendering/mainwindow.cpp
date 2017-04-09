#include "mainwindow.h"

MainWindow::MainWindow(QWindow *parent) :
    QOpenGLWindow(QOpenGLWindow::NoPartialUpdate, parent)
{
}

MainWindow::~MainWindow()
{
    makeCurrent();

    m_VAO.destroy();

    doneCurrent();
}

void MainWindow::initializeGL()
{
    m_VAO.create();
}

void MainWindow::resizeGL(int w, int h)
{

}

void MainWindow::paintGL()
{
    m_VAO.bind();

    m_VAO.release();
}
