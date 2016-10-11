#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>

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

};

#endif // MAINWINDOW_H
