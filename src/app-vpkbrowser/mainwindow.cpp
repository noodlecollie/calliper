#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::menuOpenVpkDir()
{
    QString defaultDir = getFileDialogueDefaultDirectory();
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Choose VPK"),
                                                    defaultDir,
                                                    tr("VPK files (*.vpk)"));
    if ( fileName.isNull() )
        return;

    loadVpkDir(fileName);
}

bool MainWindow::loadVpkDir(const QString &filename)
{
    QFileInfo fileInfo(filename);
    if ( !fileInfo.exists() )
    {
        showErrorMessage(QString(tr("Unable to open file %1.")).arg(filename),
                         tr("The file does not exist."));
        return false;
    }

    m_VPKFile.setFileName(filename);

    if ( !m_VPKFile.open() )
    {
        showErrorMessage(QString(tr("Unable to read file %1.")).arg(filename));
        return false;
    }

    QString errorHint;
    if ( !m_VPKFile.readIndex(&errorHint) )
    {
        showErrorMessage(QString(tr("Unable to read VPK index from file %1."))
                         .arg(filename),
                         QString(),
                         errorHint);
        return false;
    }

    return true;
}

void MainWindow::showErrorMessage(const QString &message, const QString &information, const QString &detail)
{
    QMessageBox messageBox(this);
    messageBox.setIcon(QMessageBox::Critical);
    messageBox.setWindowTitle(tr("Error"));
    messageBox.setText(message);
    messageBox.setInformativeText(information);
    messageBox.setDetailedText(detail);
    messageBox.exec();
}

QString MainWindow::getFileDialogueDefaultDirectory() const
{
    if ( m_strLastFileOpened.isNull() )
    {
        QStringList locations = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
        if ( !locations.isEmpty() )
            return locations.at(0);
        else
            return QString();
    }
    else
    {
        return QFileInfo(m_strLastFileOpened).canonicalPath();
    }
}
