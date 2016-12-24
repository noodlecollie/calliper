#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "projectfiledockwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initDockWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initDockWidgets()
{
    ProjectFileDockWidget* w = new ProjectFileDockWidget();
    addDockWidget(Qt::LeftDockWidgetArea, w);
    w->show();
}
