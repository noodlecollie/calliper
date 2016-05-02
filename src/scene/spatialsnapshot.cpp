#include "spatialsnapshot.h"

QDebug operator <<(QDebug& debug, const SpatialSnapshot &sshot)
{
    debug.nospace() << "SpatialSnapshot(" << sshot.translation
                    << ", " << sshot.rotation
                    << ", " << sshot.scale << ")";

    return debug.space();
}
