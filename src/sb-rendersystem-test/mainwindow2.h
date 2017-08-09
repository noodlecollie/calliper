#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QMainWindow>
#include <QOpenGLContext>
#include "customopenglwidget.h"

class MainWindow2 : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow2(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());
    ~MainWindow2();

private:
    CustomOpenGLWidget* m_pOpenGLWidget;
    QOpenGLContext* m_pContext;
};

#endif // MAINWINDOW2_H
