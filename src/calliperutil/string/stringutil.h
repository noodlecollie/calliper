#ifndef STRING_H
#define STRING_H

#include "calliperutil_global.h"
#include <QByteArray>
#include <QDataStream>

namespace CalliperUtil
{
    namespace StringUtil
    {
        // Assumes the input is a QByteArray and that the input data contains
        // a null-terminated string at the current position. If a null cannot
        // be found, a null byte array is returned.
        QByteArray getNullTerminatedString(QDataStream& dataStream);
    }
}

#endif // STRING_H
