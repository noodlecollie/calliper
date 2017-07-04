#ifndef STD140SERIALISATION_H
#define STD140SERIALISATION_H

#include <QDataStream>

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

#include "std140alignment.h"

// Fun things learned today: full specialisations are no longer template functions!
// They produce duplicate symbol errors unless inlined.

namespace UniformStd140
{
    template<typename T>
    QDataStream& serialiseHelper(QDataStream& stream, const T& item)
    {
        return stream << item;
    }

    template<typename T, quint32 NUMFLOATS>
    QDataStream& serialiseMatrixHelper(QDataStream& stream, const T& matrix)
    {
        stream.writeRawData(reinterpret_cast<const char*>(matrix.constData()), NUMFLOATS * sizeof(float));
        return stream;
    }

    template<>
    inline QDataStream& serialiseHelper<QVector2D>(QDataStream& stream, const QVector2D& item)
    {
        return stream << item.x() << item.y();
    }

    template<>
    inline QDataStream& serialiseHelper<QVector3D>(QDataStream& stream, const QVector3D& item)
    {
        return stream << item.x() << item.y() << item.z();
    }

    template<>
    inline QDataStream& serialiseHelper<QVector4D>(QDataStream& stream, const QVector4D& item)
    {
        return stream << item.x() << item.y() << item.z() << item.w();
    }

    template<>
    inline QDataStream& serialiseHelper<QMatrix2x2>(QDataStream& stream, const QMatrix2x2& item)
    {
        return serialiseMatrixHelper<QMatrix2x2, 2 * 2>(stream, item);
    }

    template<>
    inline QDataStream& serialiseHelper<QMatrix2x3>(QDataStream& stream, const QMatrix2x3& item)
    {
        return serialiseMatrixHelper<QMatrix2x3, 2 * 3>(stream, item);
    }

    template<>
    inline QDataStream& serialiseHelper<QMatrix2x4>(QDataStream& stream, const QMatrix2x4& item)
    {
        return serialiseMatrixHelper<QMatrix2x4, 2 * 4>(stream, item);
    }

    template<>
    inline QDataStream& serialiseHelper<QMatrix3x2>(QDataStream& stream, const QMatrix3x2& item)
    {
        return serialiseMatrixHelper<QMatrix3x2, 3 * 2>(stream, item);
    }

    template<>
    inline QDataStream& serialiseHelper<QMatrix3x3>(QDataStream& stream, const QMatrix3x3& item)
    {
        return serialiseMatrixHelper<QMatrix3x3, 3 * 3>(stream, item);
    }

    template<>
    inline QDataStream& serialiseHelper<QMatrix3x4>(QDataStream& stream, const QMatrix3x4& item)
    {
        return serialiseMatrixHelper<QMatrix3x4, 3 * 4>(stream, item);
    }

    template<>
    inline QDataStream& serialiseHelper<QMatrix4x2>(QDataStream& stream, const QMatrix4x2& item)
    {
        return serialiseMatrixHelper<QMatrix4x2, 4 * 2>(stream, item);
    }

    template<>
    inline QDataStream& serialiseHelper<QMatrix4x3>(QDataStream& stream, const QMatrix4x3& item)
    {
        return serialiseMatrixHelper<QMatrix4x3, 4 * 3>(stream, item);
    }

    template<>
    inline QDataStream& serialiseHelper<QMatrix4x4>(QDataStream& stream, const QMatrix4x4& item)
    {
        return serialiseMatrixHelper<QMatrix4x4, 4 * 4>(stream, item);
    }

    template<typename T>
    QDataStream& serialise(QDataStream& stream, const T& item)
    {
        AlignmentInfo alignmentInfo = baseAlignment<T>();

        serialiseHelper(stream, item);

        if ( alignmentInfo.requiresPadding() )
        {
            const quint16 numPaddingBytes = alignmentInfo.baseAlignment - alignmentInfo.realSize;
            QByteArray padding(numPaddingBytes, 0);
            stream.writeRawData(padding.constData(), padding.length());
        }

        return stream;
    }

    template<typename T, quint32 N>
    QDataStream& serialiseArray(QDataStream& stream, T(&array)[N])
    {
        AlignmentInfo alignmentInfo = baseAlignment(array);

        for ( int i = 0; i < N; ++i )
        {
            serialiseHelper(stream, array[i]);

            if ( alignmentInfo.requiresPadding() )
            {
                const quint16 numPaddingBytes = alignmentInfo.baseAlignment - alignmentInfo.realSize;
                QByteArray padding(numPaddingBytes, 0);
                stream.writeRawData(padding.constData(), padding.length());
            }
        }

        return stream;
    }
}

#endif // STD140SERIALISATION_H
