#include "basetool.h"

BaseTool::BaseTool(const QString &name) : QObject(NULL)
{
    setObjectName(name);
}

BaseTool::~BaseTool()
{

}

void BaseTool::activate()
{
}

void BaseTool::deactivate()
{
}
