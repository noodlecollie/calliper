#ifndef GEOMETRYOFFSETTABLE_H
#define GEOMETRYOFFSETTABLE_H

#include <QVector>
#include <QOpenGLFunctions>

#include "rendersystem/interface-classes/geometry/geometrysection.h"

class GeometryOffsetTable
{
public:
    struct ObjectOffsets
    {
        quint32 vertexOffsetFloats;
        quint32 vertexCountFloats;
        quint32 indexOffsetInts;
        quint32 indexCountInts;
        RenderSystem::GeometrySection::DrawMode drawMode;
        float lineWidth;

        ObjectOffsets(quint32 vOffset, quint32 vCount,
                      quint32 iOffset, quint32 iCount,
                      RenderSystem::GeometrySection::DrawMode dMode = RenderSystem::GeometrySection::DrawTriangles,
                      float width = 1.0f)
            : vertexOffsetFloats(vOffset),
              vertexCountFloats(vCount),
              indexOffsetInts(iOffset),
              indexCountInts(iCount),
              drawMode(dMode),
              lineWidth(width)
        {
        }

        ObjectOffsets()
            : ObjectOffsets(0,0,0,0)
        {
        }
    };

    GeometryOffsetTable();

    int count() const;
    void clear();
    ObjectOffsets at(int index) const;

    ObjectOffsets& operator [](int index);
    const ObjectOffsets& operator [](int index) const;

    ObjectOffsets& createNewItem();
    ObjectOffsets& lastItem();

private:
    QVector<ObjectOffsets> m_Offsets;
};

#endif // GEOMETRYOFFSETTABLE_H
