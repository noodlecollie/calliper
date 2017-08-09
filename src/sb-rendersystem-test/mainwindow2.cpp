#include "mainwindow2.h"
#include <QGuiApplication>

MainWindow2::MainWindow2(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags),
      m_pOpenGLWidget(Q_NULLPTR),
      m_pContext(Q_NULLPTR)
{
    QList<QScreen*> screens = qApp->screens();
    Q_ASSERT(!screens.isEmpty());

    m_pContext = new QOpenGLContext();
    m_pContext->setFormat(QSurfaceFormat::defaultFormat());
    m_pContext->setScreen(screens.at(0));
    m_pContext->create();

    m_pOpenGLWidget = new CustomOpenGLWidget(m_pContext);
    setCentralWidget(m_pOpenGLWidget);
    setWindowTitle("MainWindow2: Electric Boogaloo");
}

MainWindow2::~MainWindow2()
{
    // Must be destroyed first.
    delete m_pOpenGLWidget;
    m_pOpenGLWidget = Q_NULLPTR;

    delete m_pContext;
    m_pContext = Q_NULLPTR;
}
