#ifndef QOBJECTUTIL_H
#define QOBJECTUTIL_H

#include "calliperutil_global.h"
#include <QObject>

namespace CalliperUtil
{
    class CALLIPERUTILSHARED_EXPORT QObjectUtil
    {
    public:
        template<typename T>
        static QString enumKeyFromValue(const QString &enumName, int value)
        {
            return enumKeyFromValue(T::staticMetaObject, enumName, value);
        }

        template<typename T>
        static QString nonNamespacedClassName()
        {
            return nonNamespacedClassName(T::staticMetaObject);
        }

        template<typename T>
        static void debugListEnums()
        {
            return debugListEnums(T::staticMetaObject);
        }

    private:
        QObjectUtil() = delete;

        static QString enumKeyFromValue(const QMetaObject& metaObject, const QString& enumName, int value);
        static QString nonNamespacedClassName(const QMetaObject& metaObject);
        static void debugListEnums(const QMetaObject& metaObject);
    };
}

#endif // QOBJECTUTIL_H
