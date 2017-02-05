#include <QApplication>
#include "user-interface/arrangeable-tabs/container/resizeablegridlayoutcontainer.h"
#include <QGroupBox>
#include <QTabBar>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UserInterface::ResizeableGridLayoutContainer container;
    QGroupBox* box = new QGroupBox();
    box->setTitle("Group Box");
    box->setWindowTitle("A Group Box");
    container.addWidget(box);

    QGroupBox* box2 = new QGroupBox();
    box2->setTitle("Group Box 2");
    box2->setWindowTitle("A second group box");
    container.addWidget(box2);

    container.show();

    return a.exec();
}
