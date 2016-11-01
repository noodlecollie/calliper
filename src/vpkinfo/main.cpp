#include <QCoreApplication>
#include "vpk/vpkheader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    NS_SERIALISATION::VPKHeader header;

    return a.exec();
}
