#include "tempclass.h"
#include <QtDebug>

TempClass::TempClass(QObject *parent) : QObject(parent)
{

}

void TempClass::handleSelect(int itemId)
{
    qDebug() << "Select received with item ID" << itemId;
}

void TempClass::handleMaximise(int itemId)
{
    qDebug() << "Maximise received with item ID" << itemId;
}

void TempClass::handleClose(int itemId)
{
    qDebug() << "Close received with item ID" << itemId;
}

void TempClass::handleFloat(int itemId, bool wasDragged)
{
    qDebug() << "Float received with item ID" << itemId << "where button"
             << (wasDragged ? "was" : "was not") << "dragged";
}
