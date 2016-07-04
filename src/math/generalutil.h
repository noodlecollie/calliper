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

    // Format (x y z)
    QVector3D vectorFromVmfCoord(const QString &coord);

    // Format (x y z) (x y z) (x y z)
    void vectorsFromVmfCoords(const QString &coords, QVector3D &v0, QVector3D &v1, QVector3D &v2);

}

#endif // GENERALUTIL_H
