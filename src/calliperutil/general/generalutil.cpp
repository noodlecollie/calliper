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
            return QString(originalPath).replace('\\', '/');
        }

        QByteArray normaliseResourcePathSeparators(const QByteArray &originalPath)
        {
            return QByteArray(originalPath).replace('\\', '/');
        }
    }
}

