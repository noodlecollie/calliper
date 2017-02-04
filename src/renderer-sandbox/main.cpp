#include <QApplication>
#include "user-interface/arrangeable-tabs/container/resizablegridlayoutcontainerbutton.h"
#include "tempclass.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TempClass c;
    UserInterface::ResizableGridLayoutContainerButton btn;
    c.connect(&btn, SIGNAL(selectInvoked()), &c, SLOT(handleSelect()));
    c.connect(&btn, SIGNAL(maximiseInvoked()), &c, SLOT(handleMaximise()));
    c.connect(&btn, SIGNAL(closeInvoked()), &c, SLOT(handleClose()));

    btn.show();

    return a.exec();
}
