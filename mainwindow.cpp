#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "copenglrenderer.h"
#include <QShowEvent>
#include "cscene.h"
#include "cdebugcube.h"

static MainWindow* g_pMainWindow = NULL;
MainWindow* appMainWindow()
{
    return g_pMainWindow;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Q_ASSERT(!g_pMainWindow);
    g_pMainWindow = this;

    ui->setupUi(this);
    m_pRenderer = new COpenGLRenderer(this);
    m_pScene = new CScene(this);

    // Add a simple debug cube to the root of the scene.
    CDebugCube* cube = new CDebugCube(m_pScene->root());
    cube->setObjectName("Debug Cube");
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

COpenGLRenderer* MainWindow::renderer() const
{
    return m_pRenderer;
}

CScene* MainWindow::scene() const
{
    return m_pScene;
}
