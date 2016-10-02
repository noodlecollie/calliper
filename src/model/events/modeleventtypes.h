#ifndef MODELEVENTTYPES_H
#define MODELEVENTTYPES_H

#include "model_global.h"
#include <QEvent>

namespace NS_MODEL
{
    enum ModelEventType
    {
        SpatialConfigurationChangeEvent = QEvent::User,
    };
}

#endif // MODELEVENTTYPES_H
