#include "application.h"
#include "mapdocument.h"
#include "mainwindow.h"
#include "tools.h"

static Application* g_pApplication = NULL;
Application* application()
{
    return g_pApplication;
}

void Application::initialise(MainWindow* win)
{
    Q_ASSERT(!g_pApplication);
    new Application(win);
}

void Application::shutdown()
{
    Q_ASSERT(g_pApplication);
    delete g_pApplication;
    g_pApplication = NULL;
}

Application::Application(MainWindow* win)
{
    g_pApplication = this;

    m_iDocumentsCreated = 0;
    m_pMainWindow = win;
}

Application::~Application()
{
    qDeleteAll(m_Documents);
    delete m_pMainWindow;
}

MapDocument* Application::document(int index) const
{
    return m_Documents.at(index);
}

int Application::documentCount() const
{
    return m_Documents.count();
}

MapDocument* Application::newDocument()
{
    MapDocument* doc = new MapDocument();
    doc->setObjectName(QString("Untitled %0").arg(++m_iDocumentsCreated));
    m_Documents.append(doc);
    m_pMainWindow->updateDocumentList(m_Documents);
    return doc;
}

void Application::closeDocument(int index)
{
    MapDocument* doc = m_Documents.takeAt(index);
    delete doc;
    m_pMainWindow->updateDocumentList(m_Documents);
}

void Application::clearDocuments()
{
    qDeleteAll(m_Documents);
    m_pMainWindow->updateDocumentList(m_Documents);
}

MainWindow* Application::mainWindow() const
{
    return m_pMainWindow.data();
}

QList<MapDocument*> Application::documents() const
{
    return m_Documents;
}
