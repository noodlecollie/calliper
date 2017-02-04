#ifndef GENERALUTIL_H
#define GENERALUTIL_H

#include "calliperutil_global.h"
#include <QString>
#include <QMetaEnum>

namespace CalliperUtil
{
    namespace General
    {
        template<typename T>
        QString enumKeyFromValue(const QString &enumName, int value)
        {
            int enumIndex = T::staticMetaObject.indexOfEnumerator(enumName.toLatin1().constData());
            QMetaEnum metaEnum = T::staticMetaObject.enumerator(enumIndex);
            return QString(metaEnum.valueToKey(value));
        }

        template<typename T>
        QString nonNamespacedClassName()
        {
            QString classname(T::staticMetaObject.className());
            int colonIndex = classname.lastIndexOf("::");

            if ( colonIndex < 0 )
                return classname;

            return classname.mid(colonIndex+2);
        }

        // As a general rule, this should be called as early as possible on an imported resource path,
        // so that the rest of the code can assume the separators are normalised.
        // Assumes no escape characters in path.
        CALLIPERUTILSHARED_EXPORT QString normaliseResourcePathSeparators(const QString& originalPath);
        CALLIPERUTILSHARED_EXPORT QByteArray normaliseResourcePathSeparators(const QByteArray& originalPath);
    }
}

#endif // GENERALUTIL_H
