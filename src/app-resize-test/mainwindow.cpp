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
    m_pGridManager = new UserInterface::ResizeableGridLayoutManager(gridLayout);
}

MainWindow::~MainWindow()
{
    delete m_pGridManager;
    delete ui;
}

void MainWindow::addNewItem()
{
    static int itemNo = 1;

    QAction* action = qobject_cast<QAction*>(sender());
    if ( !action )
        return;

    QString label = QString("Item %1").arg(itemNo++);
    QWidget* n = new QGroupBox(label);
    n->setObjectName(label);

    if ( action == ui->actionUpper_Left )
    {
        QWidget* w = m_pGridManager->insertWidget(n, UserInterface::QuadGridLayoutDefs::NorthWest, Qt::Horizontal);
        if ( w )
            delete w;
    }
    else if ( action == ui->actionUpper_Right )
    {
        QWidget* w = m_pGridManager->insertWidget(n, UserInterface::QuadGridLayoutDefs::NorthEast, Qt::Horizontal);
        if ( w )
            delete w;
    }
    else if ( action == ui->actionLower_Left )
    {
        QWidget* w = m_pGridManager->insertWidget(n, UserInterface::QuadGridLayoutDefs::SouthWest, Qt::Horizontal);
        if ( w )
            delete w;
    }
    else if ( action == ui->actionLower_Right )
    {
        QWidget* w = m_pGridManager->insertWidget(n, UserInterface::QuadGridLayoutDefs::SouthEast, Qt::Horizontal);
        if ( w )
            delete w;
    }
    else
    {
        --itemNo;
        delete n;
    }
}

void MainWindow::removeItem()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if ( !action )
        return;

    if ( action == ui->actionRemove_UL )
    {
        QWidget* w = m_pGridManager->removeWidget(UserInterface::QuadGridLayoutDefs::NorthWest, Qt::Horizontal);
        if ( w )
            delete w;
    }
    else if ( action == ui->actionRemove_UR )
    {
        QWidget* w = m_pGridManager->removeWidget(UserInterface::QuadGridLayoutDefs::NorthEast, Qt::Horizontal);
        if ( w )
            delete w;
    }
    else if ( action == ui->actionRemove_LL )
    {
        QWidget* w = m_pGridManager->removeWidget(UserInterface::QuadGridLayoutDefs::SouthWest, Qt::Horizontal);
        if ( w )
            delete w;
    }
    else if ( action == ui->actionRemove_LR )
    {
        QWidget* w = m_pGridManager->removeWidget(UserInterface::QuadGridLayoutDefs::SouthEast, Qt::Horizontal);
        if ( w )
            delete w;
    }
}

void MainWindow::equalise()
{
    m_pGridManager->equaliseCellSizes();
}
