#include "paths.h"

#include <QVector3D>
#include <QStringList>
#include <QDir>

namespace CalliperUtil
{
    namespace Paths
    {
        QString normaliseResourcePathSeparators(const QString &originalPath)
        {
            return QString(originalPath).replace('\\', '/');
        }

        QByteArray normaliseResourcePathSeparators(const QByteArray &originalPath)
        {
            return QByteArray(originalPath).replace('\\', '/');
        }
    }
}

