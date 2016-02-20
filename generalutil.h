#ifndef GENERALUTIL_H
#define GENERALUTIL_H

#include <QString>
#include <QMetaEnum>

namespace GeneralUtil
{
    template<typename T>
    QString enumKeyFromValue(const QString &enumName, int value)
    {
        int enumIndex = T::staticMetaObject.indexOfEnumerator(enumName.toLatin1().constData());
        QMetaEnum metaEnum = T::staticMetaObject.enumerator(enumIndex);
        return QString(metaEnum.valueToKey(value));
    }
}

#endif // GENERALUTIL_H
