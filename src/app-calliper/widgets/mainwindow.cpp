#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "projectfiledockwidget.h"
#include <QSignalBlocker>
#include <QStandardPaths>
#include <QFileDialog>
#include <QFileInfo>
#include "projects/calliperprojectloader.h"
#include "json/jsonloaderutils.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateWindowTitle();

    initDockWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initDockWidgets()
{
    m_pProjectFileDockWidget = new ProjectFileDockWidget();
    m_pProjectFileDockWidget->setVisibilityAction(ui->actionFile_Tree);
    addDockWidget(Qt::LeftDockWidgetArea, m_pProjectFileDockWidget);
    m_pProjectFileDockWidget->show();
}

QString MainWindow::getFileDialogueDefaultPath() const
{
    if ( !m_strProjectFileName.isNull() )
    {
        return QFileInfo(m_strProjectFileName).canonicalPath();
    }

    QStringList locations = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    if ( !locations.isEmpty() )
        return locations.at(0);
    else
        return QString();
}

void MainWindow::menuOpenProject()
{
    QString defaultPath = getFileDialogueDefaultPath();

    QString filename = QFileDialog::getOpenFileName(this, tr("Open Project"), defaultPath, tr("Calliper project (*.cpj)"));
    if ( filename.isNull() || filename.isEmpty() )
        return;

    QJsonDocument doc;
    if ( !ModelLoaders::JsonLoaderUtils::loadJsonFile(filename, doc) )
    {
        QMessageBox::critical(this, tr("Error"), QString("Unable to load project file %1").arg(filename));
        return;
    }

    m_strProjectFileName = filename;

    m_Project.clear();
    ModelLoaders::CalliperProjectLoader(&m_Project).fromJsonDocument(doc);
    updateWindowTitle();
    repopulateProjectFileTree();
}

void MainWindow::saveCurrentProject(const QString& filename)
{
    if ( filename.isNull() || filename.isEmpty() )
        return;

    QFile outFile(filename);
    if ( !outFile.open(QIODevice::WriteOnly) )
    {
        QMessageBox::critical(this, tr("Error"), QString("Unable to open project file %1 for writing").arg(filename));
        return;
    }

    outFile.write(ModelLoaders::CalliperProjectLoader(&m_Project).toJsonDocument().toJson());
    outFile.close();
    m_strProjectFileName = filename;
    updateWindowTitle();
    repopulateProjectFileTree();
}

void MainWindow::menuSaveCurrentProject()
{
    if ( m_strProjectFileName.isNull() )
        menuSaveCurrentProjectAs();
    else
        saveCurrentProject(m_strProjectFileName);
}

void MainWindow::menuSaveCurrentProjectAs()
{
    QString defaultPath = getFileDialogueDefaultPath();

    QString filename = QFileDialog::getSaveFileName(this, tr("Save Project"), defaultPath, tr("Calliper project (*.cpj)"));
    if ( filename.isNull() )
        return;

    saveCurrentProject(filename);
}

void MainWindow::updateWindowTitle()
{
    if ( hasValidProject() )
    {
        QString name = m_Project.metadata().projectName();
        if ( name.isNull() || name.isEmpty() )
            setWindowTitle(tr("[Unnamed Project]"));
        else
            setWindowTitle(name);
    }
    else
    {
        setWindowTitle(tr("[No Project]"));
    }
}

void MainWindow::repopulateProjectFileTree()
{
    m_pProjectFileDockWidget->clearFiles();
    m_pProjectFileDockWidget->setRoot(QFileInfo(m_strProjectFileName).fileName());
}

bool MainWindow::hasValidProject() const
{
    return !m_strProjectFileName.isNull();
}
