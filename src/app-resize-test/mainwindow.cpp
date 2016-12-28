#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QGridLayout>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGridLayout* gridLayout = new QGridLayout();
    ui->centralWidget->setLayout(gridLayout);
    m_pGridManager = new HighLevelConvenience::ResizeableGridLayoutManager(gridLayout);
}

MainWindow::~MainWindow()
{
    delete m_pGridManager;
    delete ui;
}

void MainWindow::addNewItem()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if ( !action )
        return;

    if ( action == ui->actionUpper_Left )
    {
        QWidget* w = m_pGridManager->takeWidget(HighLevelConvenience::ResizeableGridLayoutManager::UpperLeft);
        if ( w )
            delete w;

        m_pGridManager->addWidget(new QGroupBox("Item"), HighLevelConvenience::ResizeableGridLayoutManager::UpperLeft);
    }
    else if ( action == ui->actionUpper_Right )
    {
        QWidget* w = m_pGridManager->takeWidget(HighLevelConvenience::ResizeableGridLayoutManager::UpperRight);
        if ( w )
            delete w;

        m_pGridManager->addWidget(new QGroupBox("Item"), HighLevelConvenience::ResizeableGridLayoutManager::UpperRight);
    }
    else if ( action == ui->actionLower_Left )
    {
        QWidget* w = m_pGridManager->takeWidget(HighLevelConvenience::ResizeableGridLayoutManager::LowerLeft);
        if ( w )
            delete w;

        m_pGridManager->addWidget(new QGroupBox("Item"), HighLevelConvenience::ResizeableGridLayoutManager::LowerLeft);
    }
    else if ( action == ui->actionLower_Right )
    {
        QWidget* w = m_pGridManager->takeWidget(HighLevelConvenience::ResizeableGridLayoutManager::LowerRight);
        if ( w )
            delete w;

        m_pGridManager->addWidget(new QGroupBox("Item"), HighLevelConvenience::ResizeableGridLayoutManager::LowerRight);
    }
}
