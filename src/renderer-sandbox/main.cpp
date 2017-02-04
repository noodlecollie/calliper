#include <QApplication>
#include "user-interface/arrangeable-tabs/container/resizablegridlayoutcontainerbutton.h"
#include "tempclass.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TempClass c;
    UserInterface::ResizableGridLayoutContainerButton btn;
    btn.setLabel("Testing");
    c.connect(&btn, SIGNAL(selectInvoked(int)), &c, SLOT(handleSelect(int)));
    c.connect(&btn, SIGNAL(maximiseInvoked(int)), &c, SLOT(handleMaximise(int)));
    c.connect(&btn, SIGNAL(closeInvoked(int)), &c, SLOT(handleClose(int)));

    btn.show();

    return a.exec();
}
