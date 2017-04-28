#ifndef ITESTINTERFACE_H
#define ITESTINTERFACE_H

#include "rendersystem_global.h"

#include <QString>

namespace RenderSystem
{
    class ITestInterface
    {
    public:
        virtual ~ITestInterface() {}

        virtual QString giveMeAString() = 0;
    };
}

#endif // ITESTINTERFACE_H
