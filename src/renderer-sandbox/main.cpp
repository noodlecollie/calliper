#include <QApplication>
#include "user-interface/arrangeable-tabs/container/resizablegridlayoutcontainerbutton.h"
#include "tempclass.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TempClass c;
    UserInterface::ResizableGridLayoutContainerButton btn;
    btn.setLabel("Testing");
    btn.setDragActivationThreshold(50);
    c.connect(&btn, SIGNAL(selectInvoked(int)), &c, SLOT(handleSelect(int)));
    c.connect(&btn, SIGNAL(maximiseInvoked(int)), &c, SLOT(handleMaximise(int)));
    c.connect(&btn, SIGNAL(closeInvoked(int)), &c, SLOT(handleClose(int)));
    c.connect(&btn, SIGNAL(floatInvoked(int,bool)), &c, SLOT(handleFloat(int,bool)));

    btn.show();

    return a.exec();
}
