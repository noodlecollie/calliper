#ifndef TESTINTERFACE_H
#define TESTINTERFACE_H

#include "rendersystem_global.h"
#include "rendersystem/interfaces/itestinterface.h"

namespace RenderSystem
{
    class TestInterface : public ITestInterface
    {
    public:
        TestInterface();

        QString giveMeAString() override;
    };
}

#endif // TESTINTERFACE_H
