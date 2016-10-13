#ifndef GENERICBRUSHFACE_H
#define GENERICBRUSHFACE_H

#include "model_global.h"
#include <QVector>
#include <QVector3D>

namespace NS_MODEL
{
    class GenericBrush;

    class GenericBrushFace
    {
    public:
        GenericBrushFace(GenericBrush* parentBrush);

        GenericBrush* parentBrush() const;

        int indexAt(int index) const;
        void appendIndex(int i);
        int indexCount() const;
        void removeIndex(int index);
        QVector<int> indexList() const;
        void clearIndices();
        QVector<QVector3D> referencedBrushVertexList() const;

    private:
        GenericBrush*   m_pParentBrush;

        QVector<int>    m_BrushVertexIndices;
    };

    typedef QSharedPointer<GenericBrushFace> GenericBrushFacePointer;
}

#endif // GENERICBRUSHFACE_H
