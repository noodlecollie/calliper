#include "geometrybuilder.h"
#include <QVector4D>

namespace NS_RENDERER
{
    GeometryBuilder::GeometryBuilder(quint16 shaderId, quint32 textureId, const QMatrix4x4 &modelToWorldMatrix)
        : m_iShaderId(shaderId), m_iTextureId(textureId), m_matModelToWorld(modelToWorldMatrix)
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

    GeometrySection& GeometryBuilder::createNewSection(quint16 shaderId, quint32 textureId, const QMatrix4x4 &matrix)
    {
        m_Sections.append(GeometrySection(shaderId, textureId, matrix));
        return currentSection();
    }

    GeometrySection& GeometryBuilder::createNewSection()
    {
        m_Sections.append(GeometrySection(m_iShaderId, m_iTextureId, m_matModelToWorld));
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

    QMatrix4x4 GeometryBuilder::modelToWorldMatrix() const
    {
        return m_matModelToWorld;
    }

    void GeometryBuilder::setModelToWorldMatrix(const QMatrix4x4 &matrix)
    {
        m_matModelToWorld = matrix;
        setAllSectionMatrices();
    }

    void GeometryBuilder::setAllSectionMatrices()
    {
        for ( int i = 0; i < m_Sections.count(); i++ )
        {
            m_Sections[i].m_matModelToWorld = m_matModelToWorld;
        }
    }

    GeometrySection& GeometryBuilder::nextEmptySection()
    {
        GeometrySection& current = currentSection();
        if ( current.positionCount() < 1 && current.indexCount() < 1 )
        {
            return current;
        }

        return createNewSection();
    }

    quint16 GeometryBuilder::shaderId() const
    {
        return m_iShaderId;
    }

    void GeometryBuilder::setShaderId(quint16 id)
    {
        m_iShaderId = id;
    }

    quint32 GeometryBuilder::textureId() const
    {
        return m_iTextureId;
    }

    void GeometryBuilder::setTextureId(quint32 id)
    {
        m_iTextureId = id;
    }
}
