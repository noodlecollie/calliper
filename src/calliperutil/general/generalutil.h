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
    }
}

#endif // GENERALUTIL_H
