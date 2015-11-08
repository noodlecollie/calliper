#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "copenglrenderer.h"
#include "cscene.h"
#include "cdebugcube.h"
#include <QOffscreenSurface>

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
    m_bRenderInitRun = false;

    ui->setupUi(this);
    initOpenGL();

    m_pScene = new CScene(this);

    // Add a simple debug cube to the root of the scene.
    CDebugCube* cube = new CDebugCube(m_pScene->root());
    cube->setObjectName("Debug Cube");
}

MainWindow::~MainWindow()
{
    delete m_pScene;
    delete m_pRenderer;

    m_pSurface->destroy();
    delete m_pSurface;

    delete ui;
}

COpenGLRenderer* MainWindow::renderer() const
{
    return m_pRenderer;
}

CScene* MainWindow::scene() const
{
    return m_pScene;
}

void MainWindow::initOpenGL()
{
    m_pSurface = new QOffscreenSurface();
    m_pSurface->setFormat(QSurfaceFormat::defaultFormat());
    m_pSurface->create();

    m_pRenderer = new COpenGLRenderer(this);
    m_pRenderer->initialise(m_pSurface);
}
