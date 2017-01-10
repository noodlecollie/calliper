#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vpk/vpkfile.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void menuOpenVpkDir();

private:
    void init();
    void updateWindowTitle();
    bool loadVpkDir(const QString& filename);
    void showErrorMessage(const QString& message,
                          const QString& information = QString(),
                          const QString& detail = QString());
    QString getFileDialogueDefaultDirectory() const;
    void updateFileTree();

    Ui::MainWindow *ui;
    FileFormats::VPKFile m_VPKFile;
    QString m_strLastFileOpened;
};

#endif // MAINWINDOW_H
