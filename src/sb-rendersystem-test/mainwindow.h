#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "rendersystem/interface-classes/definitions/framebufferdefs.h"
#include "rendersystem/interface-classes/definitions/rendermodeldefs.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    RenderSystem::RenderModelDefs::RenderModelId m_nRenderModelId;
    RenderSystem::FrameBufferDefs::FrameBufferId m_nFrameBufferId;
};

#endif // MAINWINDOW_H
