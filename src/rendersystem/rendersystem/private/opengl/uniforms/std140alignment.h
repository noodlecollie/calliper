#ifndef STD140ALIGNMENT_H
#define STD140ALIGNMENT_H

#include <QPair>

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

#include <QMatrix2x2>
#include <QMatrix2x3>
#include <QMatrix2x4>
#include <QMatrix3x2>
#include <QMatrix3x3>
#include <QMatrix3x4>
#include <QMatrix4x2>
#include <QMatrix4x3>
#include <QMatrix4x4>

#include <QVector>

#include "calliperutil/math/math.h"
#include "calliperutil/array/arrayutil.h"

namespace UniformStd140
{
    struct AlignmentInfo : public QPair<quint16, quint16>
    {
        quint16& realSize;
        quint16& baseAlignment;

        AlignmentInfo(quint16 size, quint16 alignment)
            : QPair<quint16, quint16>(size, alignment),
              realSize(first),
              baseAlignment(second)
        {
        }

        AlignmentInfo(quint16 size)
            : AlignmentInfo(size, size)
        {
        }

        AlignmentInfo()
            : AlignmentInfo(0)
        {
        }

        bool requiresPadding() const
        {
            return baseAlignment > realSize;
        }
    };

    template<typename T>
    AlignmentInfo baseAlignment()
    {
        return AlignmentInfo(sizeof(T));
    }

    template<>
    inline AlignmentInfo baseAlignment<QVector2D>()
    {
        return AlignmentInfo(2 * sizeof(float));
    }

    template<>
    inline AlignmentInfo baseAlignment<QVector4D>()
    {
        return AlignmentInfo(4 * sizeof(float));
    }

    template<>
    inline AlignmentInfo baseAlignment<QVector3D>()
    {
        return AlignmentInfo(3 * sizeof(float), baseAlignment<QVector4D>().baseAlignment);
    }

    template<typename T>
    AlignmentInfo baseAlignmentArray()
    {
        static const quint16 alignmentOfVec4 = baseAlignment<QVector4D>().baseAlignment;

        const AlignmentInfo infoOfSingleElement = baseAlignment<T>();
        const quint16 roundedAlignment = CalliperUtil::Math::alignedValue(infoOfSingleElement.baseAlignment, alignmentOfVec4);
        return AlignmentInfo(infoOfSingleElement.realSize, roundedAlignment);
    }

    template<typename T, quint32 N>
    AlignmentInfo baseAlignment(T(&)[N])
    {
        return baseAlignmentArray<T>();
    }

    template<>
    inline AlignmentInfo baseAlignment<QMatrix2x2>()
    {
        return baseAlignmentArray<QVector2D>();
    }

    template<>
    inline AlignmentInfo baseAlignment<QMatrix2x3>()
    {
        return baseAlignmentArray<QVector2D>();
    }

    template<>
    inline AlignmentInfo baseAlignment<QMatrix2x4>()
    {
        return baseAlignmentArray<QVector2D>();
    }

    template<>
    inline AlignmentInfo baseAlignment<QMatrix3x2>()
    {
        return baseAlignmentArray<QVector3D>();
    }

    template<>
    inline AlignmentInfo baseAlignment<QMatrix3x3>()
    {
        return baseAlignmentArray<QVector3D>();
    }

    template<>
    inline AlignmentInfo baseAlignment<QMatrix3x4>()
    {
        return baseAlignmentArray<QVector3D>();
    }

    template<>
    inline AlignmentInfo baseAlignment<QMatrix4x2>()
    {
        return baseAlignmentArray<QVector4D>();
    }

    template<>
    inline AlignmentInfo baseAlignment<QMatrix4x3>()
    {
        return baseAlignmentArray<QVector4D>();
    }

    template<>
    inline AlignmentInfo baseAlignment<QMatrix4x4>()
    {
        return baseAlignmentArray<QVector4D>();
    }
}

#endif // STD140ALIGNMENT_H
