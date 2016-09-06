#include "geometrybuilder.h"
#include <QVector4D>

namespace NS_RENDERER
{
    GeometryBuilder::GeometryBuilder()
    {
        createNewSection();
    }

    GeometryBuilder::~GeometryBuilder()
    {
    }

    GeometrySection& GeometryBuilder::currentSection()
    {
       return m_Sections.last();
    }

    GeometrySection& GeometryBuilder::createNewSection()
    {
        m_Sections.append(GeometrySection());
        return currentSection();
    }

    int GeometryBuilder::sectionCount() const
    {
        return m_Sections.count();
    }

    void GeometryBuilder::consolidate(QVector<float> &positions, QVector<float> normals,
                                      QVector<float> &colors, QVector<float> &textureCoordiates,
                                      QVector<quint32> &indices) const
    {
        int verticesProcessed = 0;

        foreach ( const GeometrySection &section, m_Sections )
        {
            positions.append(section.vertexConstVector(GeometrySection::PositionAttribute));
            normals.append(section.vertexConstVector(GeometrySection::NormalAttribute));
            colors.append(section.vertexConstVector(GeometrySection::ColorAttribute));
            textureCoordiates.append(section.vertexConstVector(GeometrySection::TextureCoordinateAttribute));

            // We need to renumber the indices from section-specific to global.
            int oldIndexCount = indices.count();
            indices.resize(oldIndexCount + section.indexCount());
            quint32* indexData = indices.data() + oldIndexCount;
            const quint32* indexSource = section.indexConstData();

            for ( int i = 0; i < section.indexCount(); i++ )
            {
                indexData[i] = verticesProcessed + indexSource[i];
            }

            verticesProcessed += section.positionCount();
        }
    }

    const QList<GeometrySection>& GeometryBuilder::sections() const
    {
        return m_Sections;
    }
}
