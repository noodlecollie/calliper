#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include "scene/scene.h"
#include "scenerenderer/scenerenderer.h"

class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

private:
    NS_MODEL::Scene* m_pScene;
    NS_MODEL::SceneRenderer* m_pSceneRenderer;
};

#endif // MAINWINDOW_H
