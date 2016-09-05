#ifndef HASHFUNCTIONS_H
#define HASHFUNCTIONS_H

#include "util_global.h"
#include <QHash>
#include <QMatrix4x4>

namespace NS_UTIL
{
    UTILSHARED_EXPORT uint qHash(const QMatrix4x4 &mat, uint seed);
}

#endif // HASHFUNCTIONS_H
