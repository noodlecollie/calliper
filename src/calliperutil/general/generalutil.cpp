#include "generalutil.h"

#include <QVector3D>
#include <QStringList>
#include <QDir>

namespace CalliperUtil
{
    namespace General
    {
        QString normaliseResourcePathSeparators(const QString &originalPath)
        {
            return QDir::fromNativeSeparators(originalPath);
        }
    }
}

