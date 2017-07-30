#ifndef MAINWINDOWSLAVEWIDGET_H
#define MAINWINDOWSLAVEWIDGET_H

#include "user-interface_global.h"

#include <QWidget>

namespace UserInterface
{
    // Is closed when QApplication::lastWindowClosed() is emitted.
    class USERINTERFACESHARED_EXPORT MainWindowSlaveWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit MainWindowSlaveWidget(QWidget *parent = 0);

    signals:

    public slots:
    };
}

#endif // MAINWINDOWSLAVEWIDGET_H
