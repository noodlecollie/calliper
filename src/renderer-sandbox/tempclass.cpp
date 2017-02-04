#include "tempclass.h"
#include <QtDebug>

TempClass::TempClass(QObject *parent) : QObject(parent)
{

}

void TempClass::handleSelect()
{
    qDebug() << "Select received";
}

void TempClass::handleMaximise()
{
    qDebug() << "Maximise received";
}

void TempClass::handleClose()
{
    qDebug() << "Close received";
}
