#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>

// Custom implementation of QOpenGLWidget, to allow a context to be specified.
// The context should be valid for the lifetime of the widget.
class OpenGLWidget : public QWidget
{
    Q_OBJECT
public:
    OpenGLWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~OpenGLWidget();

    void makeCurrent();
    void doneCurrent();
};

#endif // OPENGLWIDGET_H
