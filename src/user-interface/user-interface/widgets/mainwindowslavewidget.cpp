#include "mainwindowslavewidget.h"

#include <QApplication>

namespace UserInterface
{
    MainWindowSlaveWidget::MainWindowSlaveWidget(QWidget *parent)
        : QWidget(parent)
    {
        setAttribute(Qt::WA_QuitOnClose, false);

        connect(qApp, &QApplication::lastWindowClosed, this, &MainWindowSlaveWidget::close);
    }
}
