#ifndef HASHFUNCTIONS_H
#define HASHFUNCTIONS_H

#include "calliperutil_global.h"
#include <QHash>
#include <QMatrix4x4>

// These must be in the global namespace.
CALLIPERUTILSHARED_EXPORT uint qHash(const QMatrix4x4 &mat, uint seed);

#endif // HASHFUNCTIONS_H
