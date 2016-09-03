#include "spatialsnapshot.h"

namespace NS_MODEL
{
    QDebug operator <<(QDebug& debug, const SpatialSnapshot &sshot)
    {
        debug.nospace() << "SpatialSnapshot(" << sshot.translation
                        << ", " << sshot.rotation
                        << ", " << sshot.scale << ")";

        return debug.space();
    }
}
