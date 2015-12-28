#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGuiApplication>
#include "application.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_iActiveDocument = -1;
    updateDocumentList(QList<MapDocument*>());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::quit()
{
    qApp->quit();
}

void MainWindow::updateDocumentList(const QList<MapDocument *> &docs)
{
    ui->menuWindow->clear();

    if ( docs.count() < 1 )
    {
        ui->menuWindow->addAction("No documents")->setEnabled(false);
        return;
    }

    for (int i = 0; i < docs.count(); i++)
    {
        MapDocument* doc = docs.at(i);
        QString text = QString("%0%1").arg(doc->objectName()).arg(i == m_iActiveDocument ? " [Active]" : "");
        QAction* a = ui->menuWindow->addAction(text);
        a->setProperty("documentIndex", QVariant(i));
        a->connect(a, SIGNAL(triggered(bool)), this, SLOT(makeDocumentActiveFromMenu()));
    }
}

void MainWindow::createNewDocument()
{
    m_iActiveDocument = application()->documentCount();
    application()->newDocument();
    updateFromActiveDocument();
}

void MainWindow::makeDocumentActiveFromMenu()
{
    QAction* action = qobject_cast<QAction*>(sender());
    Q_ASSERT(action);

    bool ok = false;
    int index = action->property("documentIndex").toInt(&ok);
    Q_ASSERT(ok);

    m_iActiveDocument = index;
    updateDocumentList(application()->documents());
    updateFromActiveDocument();
}

void MainWindow::closeActiveDocument()
{
    if ( m_iActiveDocument < 0 ) return;

    int old = m_iActiveDocument;
    if ( m_iActiveDocument == application()->documentCount() - 1 )
    {
        m_iActiveDocument--;
    }

    application()->closeDocument(old);
    updateFromActiveDocument();
}

void MainWindow::updateFromActiveDocument()
{
    MapDocument* doc = activeDocument();

    if ( !doc )
    {
        ui->viewport->setBackgroundColor(Viewport::defaultBackgroundColor());
        return;
    }

    ui->viewport->setBackgroundColor(doc->backgroundColor());
}

MapDocument* MainWindow::activeDocument() const
{
    Q_ASSERT(m_iActiveDocument < application()->documentCount());

    return m_iActiveDocument < 0 ? NULL : application()->document(m_iActiveDocument);
}
