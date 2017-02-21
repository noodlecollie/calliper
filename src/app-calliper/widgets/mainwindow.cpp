#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "projectfiledockwidget.h"
#include <QSignalBlocker>
#include <QStandardPaths>
#include <QFileDialog>
#include <QFileInfo>
#include "model-loaders/projects/calliperprojectloader.h"
#include "model-loaders/json/jsonloaderutils.h"
#include <QMessageBox>
#include "projectmetadatadockwidget.h"
#include <QtDebug>
#include "user-interface/arrangeable-tabs/widget/quadgridwidget.h"

// Swap me out for something useful
#include <QGroupBox>

namespace AppCalliper
{
    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        m_bUnsavedProjectChanges(false)
    {
        ui->setupUi(this);

        initDockWidgets();
        connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
        setProject(nullptr);
    }

    MainWindow::~MainWindow()
    {
        delete ui;
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

        setProject(new ApplicationProject());

        m_pProject->setFileName(filename);
        ModelLoaders::CalliperProjectLoader(m_pProject->project()).fromJsonDocument(doc);

        m_bUnsavedProjectChanges = false;
        updateWindowTitle();
        repopulateProjectFileTree();
    }

    void MainWindow::menuCloseProject()
    {
        if ( !ensureProjectIsSaved() )
            return;

        setProject(nullptr);
        repopulateProjectFileTree();
    }

    void MainWindow::menuNewProject()
    {
        if ( !ensureProjectIsSaved() )
            return;

        setProject(new ApplicationProject());
        menuSaveCurrentProjectAs();

        if ( m_pProject->fileName().isNull() )
            setProject(nullptr);
    }

    void MainWindow::menuSaveCurrentProject()
    {
        if ( !m_pProject )
            return;

        if ( m_pProject->fileName().isNull() )
            menuSaveCurrentProjectAs();
        else
            saveCurrentProject(m_pProject->fileName());
    }

    void MainWindow::menuSaveCurrentProjectAs()
    {
        if ( !m_pProject )
            return;

        QString defaultPath = getFileDialogueDefaultPath();

        QString filename = QFileDialog::getSaveFileName(this, tr("Save Project"), defaultPath, tr("Calliper project (*.cpj)"));
        if ( filename.isNull() || filename.isEmpty() )
            return;

        saveCurrentProject(filename);
    }

    bool MainWindow::ensureProjectIsSaved()
    {
        if ( m_bUnsavedProjectChanges )
        {
            QMessageBox::StandardButton ret =
                    QMessageBox::question(this,
                                          tr("Unsaved Changes"),
                                          tr("Project has unsaved changes. Save now?"),
                                          QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                          QMessageBox::Yes);

            if ( ret == QMessageBox::Cancel )
            {
                return false;
            }
            else if ( ret == QMessageBox::Yes )
            {
                menuSaveCurrentProject();

                // Make sure it succeeded!
                if ( m_bUnsavedProjectChanges )
                    return false;
            }
        }

        return true;
    }

    void MainWindow::saveCurrentProject(const QString& filename)
    {
        if ( filename.isNull() || filename.isEmpty() || !m_pProject )
            return;

        QFile outFile(filename);
        if ( !outFile.open(QIODevice::WriteOnly) )
        {
            QMessageBox::critical(this, tr("Error"), QString("Unable to open project file %1 for writing").arg(filename));
            return;
        }

        outFile.write(ModelLoaders::CalliperProjectLoader(m_pProject->project()).toJsonDocument().toJson());
        outFile.close();

        m_bUnsavedProjectChanges = false;
        m_pProject->setFileName(filename);
        m_pProject->notifyDataReset();
        updateWindowTitle();
        repopulateProjectFileTree();
    }

    void MainWindow::updateWindowTitle()
    {
        if ( m_pProject )
        {
            QString name = m_pProject->project()->metadata()->projectName();
            QString title;

            if ( name.isNull() || name.isEmpty() )
                title = tr("[Unnamed Project]");
            else
                title = name;

            if ( m_bUnsavedProjectChanges )
                title += "*";

            setWindowTitle(title);
        }
        else
        {
            setWindowTitle(tr("[No Project]"));
        }
    }

    void MainWindow::repopulateProjectFileTree()
    {
        m_pProjectFileDockWidget->clearFiles();

        if ( !m_pProject )
            return;

        QString filename = m_pProject->fileName();
        if ( filename.isNull() )
        {
            filename = "UNSAVED";
        }

        m_pProjectFileDockWidget->setRoot(QFileInfo(filename).fileName());
    }

    void MainWindow::setProject(ApplicationProject *newProject)
    {
        m_pProjectMetadataDockWidget->setProjectMetadata(nullptr);

        m_pProject.reset(newProject);
        m_bUnsavedProjectChanges = false;
        updateWindowTitle();
        setFileMenuItemEnabledStates();

        if ( m_pProject )
        {
            connect(m_pProject.data(), SIGNAL(dataChanged()), this, SLOT(notifyProjectDataChanged()));

            m_pProjectMetadataDockWidget->setProjectMetadata(m_pProject->project()->metadata());
        }
    }

    void MainWindow::notifyProjectDataChanged()
    {
        m_bUnsavedProjectChanges = true;
        updateWindowTitle();
    }

    void MainWindow::initDockWidgets()
    {
        m_pProjectFileDockWidget = new ProjectFileDockWidget();
        initDockWidget(m_pProjectFileDockWidget, ui->actionFile_Tree, Qt::LeftDockWidgetArea);

        m_pProjectMetadataDockWidget = new ProjectMetadataDockWidget();
        initDockWidget(m_pProjectMetadataDockWidget, ui->actionProject_Metadata, Qt::LeftDockWidgetArea);
    }

    void MainWindow::initDockWidget(VisibleActionDockWidget *widget, QAction *action, Qt::DockWidgetArea area)
    {
        widget->setVisibilityAction(action);
        addDockWidget(area, widget);
        widget->setVisible(action->isChecked());
    }

    QString MainWindow::getFileDialogueDefaultPath() const
    {
        if ( m_pProject && !m_pProject->fileName().isNull() )
        {
            return QFileInfo(m_pProject->fileName()).canonicalPath();
        }

        QStringList locations = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
        if ( !locations.isEmpty() )
            return locations.at(0);
        else
            return QString();
    }

    void MainWindow::setFileMenuItemEnabledStates()
    {
        bool haveProject = !m_pProject.isNull();

        ui->actionClose_Project->setEnabled(haveProject);
        ui->actionNew_Project->setEnabled(true);
        ui->actionOpen_Project->setEnabled(true);
        ui->actionSave_Project->setEnabled(haveProject);
        ui->actionSave_Project_As->setEnabled(haveProject);
    }

    UserInterface::QuadGridWidget* MainWindow::centralGridWidget() const
    {
        return qobject_cast<UserInterface::QuadGridWidget*>(centralWidget());
    }

    void MainWindow::fileDoubleClicked(const QString& path)
    {
        // TODO: This is all placeholder code. Rewrite this to be flexible.

        QFileInfo fileInfo(path);
        if ( fileInfo.suffix() != "vmf" )
        {
            return;
        }

        QGroupBox* temp = new QGroupBox();
        temp->setTitle(path);

        UserInterface::QuadGridWidget* gridWidget = centralGridWidget();
        gridWidget->setSingleWidget(temp);
    }
}
