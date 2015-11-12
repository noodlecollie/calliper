#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "copenglrenderer.h"
#include "cscene.h"
#include "cdebugcube.h"
#include "cresourcemanager.h"
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
    
    m_pResourceManager = new CResourceManager(this);
    m_pResourceManager->backgroundContext()->makeCurrent(m_pResourceManager->generalSurface());
    m_pResourceManager->addShader("SolidColor", ":/shaders/plain.vert", ":/shaders/solidcolor.frag");
    m_pResourceManager->loadTexture(QUrl("qrc:/test.png"));
    m_pResourceManager->backgroundContext()->doneCurrent();
    
    m_pRenderer = new COpenGLRenderer(m_pResourceManager, this);
    m_pRenderer->setRenderColor(QColor(255,128,0));
    
    m_pScene = new CScene(this);

    // Add a simple debug cube to the root of the scene.
//    CDebugCube* cube = new CDebugCube(m_pScene->root());
//    cube->setObjectName("Debug Cube");
//    cube->setPosition(QVector3D(0,0,0));
    
    for ( float x = -5.0f; x <= 5.0f; x += 0.5f )
    {
        for ( float y = -5.0f; y <= 5.0f; y += 0.5f )
        {
            for ( float z = -5.0f; z <= 5.0f; z += 0.5f )
            {
                CDebugCube* c = new CDebugCube(m_pScene->root());
                c->setPosition(QVector3D(x,y,z));
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete m_pScene;
    delete m_pRenderer;
    delete m_pResourceManager;

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

CResourceManager* MainWindow::resourceManager() const
{
    return m_pResourceManager;
}
