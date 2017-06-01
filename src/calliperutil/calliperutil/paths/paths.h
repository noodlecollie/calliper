#ifndef PATHS_H
#define PATHS_H

#include "calliperutil_global.h"

#include <QString>

namespace CalliperUtil
{
    namespace Paths
    {
        // As a general rule, this should be called as early as possible on an imported resource path,
        // so that the rest of the code can assume the separators are normalised.
        // Assumes no escape characters in path.
        CALLIPERUTILSHARED_EXPORT QString normaliseResourcePathSeparators(const QString& originalPath);
        CALLIPERUTILSHARED_EXPORT QByteArray normaliseResourcePathSeparators(const QByteArray& originalPath);
    }
}

#endif // PATHS_H
