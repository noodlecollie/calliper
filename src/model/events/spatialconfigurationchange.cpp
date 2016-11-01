#include "spatialconfigurationchange.h"
#include "modeleventtypes.h"

namespace NS_MODEL
{
    SpatialConfigurationChange::SpatialConfigurationChange(ChangeType changeType)
        : QEvent(static_cast<QEvent::Type>(SpatialConfigurationChangeEvent)),
          m_iChangeType(changeType)
    {

    }

    SpatialConfigurationChange::ChangeType SpatialConfigurationChange::changeType() const
    {
        return m_iChangeType;
    }
}
