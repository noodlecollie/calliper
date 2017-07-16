#ifndef IPRESETMATERIALINITIALISER_H
#define IPRESETMATERIALINITIALISER_H

#include <QSharedPointer>
#include "rendersystem/interface-classes/rendermaterial/rendermaterial.h"

class IPresetMaterialInitialiser
{
public:
    virtual ~IPresetMaterialInitialiser() {}

    virtual QString name() const = 0;
    virtual void initialise(const QSharedPointer<RenderSystem::RenderMaterial>& material) const = 0;
};

#endif // IPRESETMATERIALINITIALISER_H
