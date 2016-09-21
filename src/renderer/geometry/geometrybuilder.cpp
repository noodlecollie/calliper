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
                                      QVector<float> &colors, QVector<float> &textureCoordinates,
                                      QVector<quint32> &indices) const
    {
        foreach ( const GeometrySection &section, m_Sections )
        {
            section.consolidate(positions, normals, colors, textureCoordinates, indices);
        }
    }

    const QList<GeometrySection>& GeometryBuilder::sections() const
    {
        return m_Sections;
    }
}
