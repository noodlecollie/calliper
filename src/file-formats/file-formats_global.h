#ifndef FILEFORMATS_GLOBAL_H
#define FILEFORMATS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FILEFORMATS_LIBRARY)
#  define FILEFORMATSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FILEFORMATSSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace FileFormats
{
    static const char* STR_NAMESPACE = "FileFormats";
}

#endif // FILEFORMATS_GLOBAL_H
