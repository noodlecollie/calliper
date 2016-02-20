#ifndef JSONUTIL_H
#define JSONUTIL_H

#include <QVector3D>
#include <QJsonArray>
#include <QVector>
#include <QColor>

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
}

#endif // JSONUTIL_H
