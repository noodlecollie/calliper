#ifndef GENERALUTIL_H
#define GENERALUTIL_H

#include "calliperutil_global.h"
#include <QString>
#include <QMetaEnum>

namespace NS_CALLIPERUTIL
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
    }
}

#endif // GENERALUTIL_H
