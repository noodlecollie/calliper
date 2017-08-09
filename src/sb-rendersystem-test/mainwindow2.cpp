#include "mainwindow2.h"

MainWindow2::MainWindow2(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags),
      m_pOpenGLWidget(Q_NULLPTR)
{
//    m_pOpenGLWidget = new CustomOpenGLWidget();
//    setCentralWidget(m_pOpenGLWidget);
    setWindowTitle("MainWindow2: Electric Boogaloo");
}
