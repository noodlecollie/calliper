#include "qobjectutil.h"
#include <QMetaEnum>
#include <QtDebug>

namespace CalliperUtil
{
    QString QObjectUtil::enumKeyFromValue(const QMetaObject &metaObject, const QString &enumName, int value)
    {
        int enumIndex = metaObject.indexOfEnumerator(qPrintable(enumName));
        QMetaEnum metaEnum = metaObject.enumerator(enumIndex);
        return QString(metaEnum.valueToKey(value));
    }

    QString QObjectUtil::nonNamespacedClassName(const QMetaObject &metaObject)
    {
        QString classname(metaObject.className());
        int colonIndex = classname.lastIndexOf("::");

        if ( colonIndex < 0 )
            return classname;

        return classname.mid(colonIndex+2);
    }

    void QObjectUtil::debugListEnums(const QMetaObject &metaObject)
    {
        qDebug() << "Class" << QString(metaObject.className()) << "has" << metaObject.enumeratorCount() << "enums:";

        for ( int i = 0; i < metaObject.enumeratorCount(); ++i )
        {
            qDebug() << QString(metaObject.enumerator(i).name());
        }
    }
}
