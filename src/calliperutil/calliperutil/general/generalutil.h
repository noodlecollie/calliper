#ifndef GENERALUTIL_H
#define GENERALUTIL_H

#include "calliperutil_global.h"
#include <QString>
#include <QMetaEnum>

namespace CalliperUtil
{
    namespace General
    {
        // As a general rule, this should be called as early as possible on an imported resource path,
        // so that the rest of the code can assume the separators are normalised.
        // Assumes no escape characters in path.
        CALLIPERUTILSHARED_EXPORT QString normaliseResourcePathSeparators(const QString& originalPath);
        CALLIPERUTILSHARED_EXPORT QByteArray normaliseResourcePathSeparators(const QByteArray& originalPath);
    }
}

#endif // GENERALUTIL_H
