#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "copenglrenderer.h"
#include <QShowEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pRenderer = new COpenGLRenderer(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *e)
{
    if ( e->spontaneous() && !m_pRenderer->initialiseAttempted() )
        m_pRenderer->initialise();
}
