#ifndef HASHFUNCTIONS_H
#define HASHFUNCTIONS_H

#include "calliperutil_global.h"
#include <QHash>
#include <QMatrix4x4>
#include <QVector3D>

// These must be in the global namespace.
CALLIPERUTILSHARED_EXPORT uint qHash(const QMatrix4x4 &mat, uint seed = 0);
CALLIPERUTILSHARED_EXPORT uint qHash(const QVector3D &vec, uint seed = 0);

#endif // HASHFUNCTIONS_H
