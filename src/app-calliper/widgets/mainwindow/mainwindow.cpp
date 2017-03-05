#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/projectfiles/projectfiledockwidget.h"
#include <QSignalBlocker>
#include <QStandardPaths>
#include <QFileDialog>
#include <QFileInfo>
#include "model-loaders/projects/calliperprojectloader.h"
#include "model-loaders/json/jsonloaderutils.h"
#include <QMessageBox>
#include "widgets/projectmetadata/projectmetadatadockwidget.h"
#include <QtDebug>
#include "user-interface/arrangeable-tabs/widget/quadgridwidget.h"
#include "model-loaders/filedataloaders/fileextensiondatamodelmap.h"
#include "user-interface/modelviews/modelviewfactory.h"

namespace AppCalliper
{
    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        m_bUnsavedProjectChanges(false)
    {
        ui->setupUi(this);

        initDockWidgets();
        connect(ui->actionQuit, &QAction::triggered, qApp, &QApplication::quit);
        setProject(Q_NULLPTR);
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }

    void MainWindow::menuOpenProject()
    {
        QString defaultPath = getFileDialogueDefaultPath();

        QString filePath = QFileDialog::getOpenFileName(this, tr("Open Project"), defaultPath, tr("Calliper project (*.cpj)"));
        if ( filePath.isNull() || filePath.isEmpty() )
            return;

        QJsonDocument doc;
        if ( !ModelLoaders::JsonLoaderUtils::loadJsonFile(filePath, doc) )
        {
            QMessageBox::critical(this, tr("Error"), QString("Unable to load project file %1").arg(filePath));
            return;
        }

        setProject(new ApplicationProject());

        m_pProject->setFullPath(filePath);
        ModelLoaders::CalliperProjectLoader(m_pProject->project()).fromJsonDocument(doc);

        m_bUnsavedProjectChanges = false;
        updateWindowTitle();
        repopulateProjectFileTree();
    }

    void MainWindow::menuCloseProject()
    {
        if ( !ensureProjectIsSaved() )
            return;

        setProject(Q_NULLPTR);
        repopulateProjectFileTree();
    }

    void MainWindow::menuNewProject()
    {
        if ( !ensureProjectIsSaved() )
            return;

        setProject(new ApplicationProject());
        menuSaveCurrentProjectAs();

        if ( m_pProject->fullPath().isNull() )
            setProject(Q_NULLPTR);
    }

    void MainWindow::menuSaveCurrentProject()
    {
        if ( !m_pProject )
            return;

        if ( m_pProject->fullPath().isNull() )
            menuSaveCurrentProjectAs();
        else
            saveCurrentProject(m_pProject->fullPath());
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

    void MainWindow::saveCurrentProject(const QString& fullPath)
    {
        if ( fullPath.isNull() || fullPath.isEmpty() || !m_pProject )
            return;

        QFile outFile(fullPath);
        if ( !outFile.open(QIODevice::WriteOnly) )
        {
            QMessageBox::critical(this, tr("Error"), QString("Unable to open project file %1 for writing").arg(fullPath));
            return;
        }

        outFile.write(ModelLoaders::CalliperProjectLoader(m_pProject->project()).toJsonDocument().toJson());
        outFile.close();

        m_bUnsavedProjectChanges = false;
        m_pProject->setFullPath(fullPath);
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

        QString fullPath = m_pProject->fullPath();
        if ( fullPath.isNull() )
        {
            fullPath = tr("UNSAVED");
        }

        m_pProjectFileDockWidget->setProject(fullPath);

        const QSet<QString>& files = m_pProject->project()->projectFiles();
        ModelLoaders::FileExtensionDataModelMap extMap;

        foreach ( const QString& path, files )
        {
            QString extension = QFileInfo(path).suffix();
            m_pProjectFileDockWidget->addFile(extMap.modelType(extension), path);
        }
    }

    void MainWindow::setProject(ApplicationProject *newProject)
    {
        m_pProjectMetadataDockWidget->setProjectMetadata(Q_NULLPTR);

        m_pProject.reset(newProject);
        m_bUnsavedProjectChanges = false;
        updateWindowTitle();
        setFileMenuItemEnabledStates();

        if ( m_pProject )
        {
            connect(m_pProject.data(), &ApplicationProject::dataChanged, this, &MainWindow::notifyProjectDataChanged);

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

        connect(m_pProjectFileDockWidget, &ProjectFileDockWidget::fileDoubleClicked, this, &MainWindow::fileDoubleClicked);
        connect(m_pProjectFileDockWidget, &ProjectFileDockWidget::addNewFileRequested, this, &MainWindow::addNewProjectFiles);

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
        if ( m_pProject && !m_pProject->fullPath().isNull() )
        {
            return QFileInfo(m_pProject->fullPath()).canonicalPath();
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

    void MainWindow::fileDoubleClicked(const QString& localPath)
    {
        if ( m_pProject.isNull() )
        {
            return;
        }

        QString fullPath = getFullPath(localPath);

        ModelLoaders::FileDataModelStore& files = m_pProject->fileStore();
        if ( files.isFileLoaded(fullPath) )
        {
            return;
        }

        QString errorString;
        ModelLoaders::BaseFileLoader::SuccessCode success = files.loadFile(fullPath, &errorString);

        switch ( success )
        {
            case ModelLoaders::BaseFileLoader::Failure:
            {
                QMessageBox box(QMessageBox::Critical,
                                tr("Error loading file"),
                                tr("There was a critical error loading the file %1").arg(fullPath),
                                QMessageBox::Ok);

                box.setDetailedText(errorString);
                box.exec();

                return;
            }

            case ModelLoaders::BaseFileLoader::PartialSuccess:
            {
                QMessageBox box(QMessageBox::Warning,
                                tr("Error loading file"),
                                tr("File %1 was loaded, but some errors occurred.").arg(fullPath),
                                QMessageBox::Ok);

                box.setInformativeText(tr("See details."));
                box.setDetailedText(errorString);
                box.exec();

                break;
            }

            default:
                break;
        }

        QSharedPointer<Model::BaseFileDataModel> dataModel = files.dataModel(fullPath);
        Q_ASSERT_X(!dataModel.isNull(), Q_FUNC_INFO, "Expected a valid data model!");
        if ( dataModel.isNull() )
        {
            QMessageBox box(QMessageBox::Critical,
                            tr("Error opening file"),
                            tr("Data model expected to be valid!"),
                            QMessageBox::Ok);

            box.exec();
            return;
        }

        QWidget* view = UserInterface::ModelViewFactory::createView(dataModel->type());
        Q_ASSERT(view);
        view->setWindowTitle(QFileInfo(fullPath).fileName());
        ui->gridWidget->setSingleWidget(view);
    }

    QString MainWindow::getFullPath(const QString &localFilePath) const
    {
        if ( m_pProject.isNull() || m_pProject->fullPath().isEmpty() )
        {
            return QString();
        }

        QFileInfo file(localFilePath);
        QDir dir(QFileInfo(m_pProject->fullPath()).canonicalPath());
        dir.cd(file.path());    // In case there's a file name on the end of the path.
        return dir.canonicalPath() + "/" + file.fileName();
    }

    void MainWindow::addNewProjectFiles()
    {
        if ( m_pProject.isNull() )
        {
            return;
        }

        QString defaultPath = getFileDialogueDefaultPath();
        ModelLoaders::FileExtensionDataModelMap extMap;

        QStringList filePathList = QFileDialog::getOpenFileNames(
                    this,
                    tr("Add File"),
                    defaultPath,
                    extMap.fileDialogTypeStrings().join(";;"));

        if ( filePathList.isEmpty() )
        {
            return;
        }

        QStringList localPaths = absoluteFilePathsToLocalFilePaths(filePathList);

        foreach ( const QString& path, localPaths )
        {
            QString extension = QFileInfo(path).suffix();
            m_pProject->project()->addProjectFile(path);
            m_pProjectFileDockWidget->addFile(extMap.modelType(extension), path);
        }
    }

    QStringList  MainWindow::absoluteFilePathsToLocalFilePaths(const QStringList &filePaths) const
    {
        QDir dir(QFileInfo(m_pProject->fullPath()).canonicalPath());
        QStringList localPaths;

        foreach ( const QString& filePath, filePaths )
        {
            localPaths.append(dir.relativeFilePath(filePath));
        }

        return localPaths;
    }
}
