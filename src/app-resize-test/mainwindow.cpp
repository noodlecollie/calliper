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
    connect(m_pGridManager, &UserInterface::ResizeableGridLayoutManager::widgetFloated, this, &MainWindow::widgetFloated);
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
    n->setWindowTitle(QString("This GroupBox is named '%1'").arg(label));

    if ( action == ui->actionUpper_Left )
    {
        m_pGridManager->addWidget(n, UserInterface::QuadGridLayoutDefs::NorthWest, Qt::Horizontal);
    }
    else if ( action == ui->actionUpper_Right )
    {
        m_pGridManager->addWidget(n, UserInterface::QuadGridLayoutDefs::NorthEast, Qt::Horizontal);
    }
    else if ( action == ui->actionLower_Left )
    {
        m_pGridManager->addWidget(n, UserInterface::QuadGridLayoutDefs::SouthWest, Qt::Horizontal);
    }
    else if ( action == ui->actionLower_Right )
    {
        m_pGridManager->addWidget(n, UserInterface::QuadGridLayoutDefs::SouthEast, Qt::Horizontal);
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
        m_pGridManager->takeActiveWidget(UserInterface::QuadGridLayoutDefs::NorthWest, Qt::Horizontal);
    }
    else if ( action == ui->actionRemove_UR )
    {
        m_pGridManager->takeActiveWidget(UserInterface::QuadGridLayoutDefs::NorthEast, Qt::Horizontal);
    }
    else if ( action == ui->actionRemove_LL )
    {
        m_pGridManager->takeActiveWidget(UserInterface::QuadGridLayoutDefs::SouthWest, Qt::Horizontal);
    }
    else if ( action == ui->actionRemove_LR )
    {
        m_pGridManager->takeActiveWidget(UserInterface::QuadGridLayoutDefs::SouthEast, Qt::Horizontal);
    }
}

void MainWindow::equalise()
{
    m_pGridManager->equaliseCellSizes();
}

void MainWindow::widgetFloated(QWidget *widget, const QPoint &globalPos, bool dragged)
{
    MainWindow* other = new MainWindow();
    other->setWidgetInGrid(widget);
    other->show();
}

void MainWindow::setWidgetInGrid(QWidget *widget)
{
    m_pGridManager->addWidget(widget, UserInterface::QuadGridLayoutDefs::NorthWest, Qt::Horizontal);
}
