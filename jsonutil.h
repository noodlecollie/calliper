#ifndef JSONUTIL_H
#define JSONUTIL_H

#include <QVector3D>
#include <QJsonArray>
#include <QVector>
#include <QColor>
#include <QMetaEnum>

namespace JsonUtil
{
    // Works with any class that has operator [] defined.
    template<typename T>
    void vector3ToJsonArray(const T &vec, QJsonArray &arr)
    {
        arr.append(QJsonValue(vec[0]));
        arr.append(QJsonValue(vec[1]));
        arr.append(QJsonValue(vec[2]));
    }

    // Works with any class that has a 3-float constructor.
    template<typename T>
    T jsonArrayToVector3(const QJsonArray &arr)
    {
        return T((float)arr.at(0).toDouble(),
                (float)arr.at(1).toDouble(),
                (float)arr.at(2).toDouble());
    }

    template<typename T, typename C>
    void vectorToJsonArray(const QVector<T> &vec, QJsonArray &arr)
    {
        for ( int i = 0; i < vec.count(); i++ )
        {
            arr.append(QJsonValue((C)vec.at(i)));
        }
    }

    template<typename T>
    QVector<T> floatJsonArrayToVector(const QJsonArray &arr)
    {
        QVector<T> vec;

        for ( int i = 0; i < arr.count(); i++ )
        {
            vec.append((T)arr.at(i).toDouble());
        }

        return vec;
    }

    template<typename T>
    QVector<T> intJsonArrayToVector(const QJsonArray &arr)
    {
        QVector<T> vec;

        for ( int i = 0; i < arr.count(); i++ )
        {
            vec.append((T)arr.at(i).toInt());
        }

        return vec;
    }

    void colorToJsonArray(const QColor &col, QJsonArray &arr);
    QColor jsonArrayToColor(const QJsonArray &arr);

    template<typename C, typename T>
    void flagsToJsonArray(const T &flags, const QString &flagName, QJsonArray &arr)
    {
        int enumIndex = C::staticMetaObject.indexOfEnumerator(flagName.toLatin1().constData());
        QMetaEnum metaEnum = C::staticMetaObject.enumerator(enumIndex);
        int keyCount = metaEnum.keyCount();

        for ( int i = 0; i < keyCount; i++ )
        {
            // Get the numeric value for the key of this index.
            int value = metaEnum.value(i);

            // Don't process zero-flags.
            if ( value == 0 )
                continue;

            // See if this flag is set.
            if ( static_cast<typename T::Int>(flags & value) == value )
            {
                // Add it to the array.
                arr.append(QJsonValue(QString(metaEnum.valueToKey(value))));
            }
        }
    }
}

#endif // JSONUTIL_H
