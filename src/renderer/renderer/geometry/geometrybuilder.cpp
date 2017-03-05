#include "geometrybuilder.h"
#include <QVector4D>

namespace Renderer
{
    GeometryBuilder::GeometryBuilder(IShaderRetrievalFunctor* shaderFunctor, ITextureRetrievalFunctor* textureFunctor,
                                     quint16 shaderId, quint32 materialId, const QMatrix4x4 &modelToWorldMatrix)
        : m_pShaderFunctor(shaderFunctor), m_pTextureFunctor(textureFunctor),
          m_iShaderId(shaderId), m_iMaterialId(materialId), m_matModelToWorld(modelToWorldMatrix)
    {
    }

    GeometryBuilder::~GeometryBuilder()
    {
        qDeleteAll(m_Sections);
    }

    GeometrySection* GeometryBuilder::currentSection()
    {
        return m_Sections.count() > 0 ? m_Sections.last() : Q_NULLPTR;
    }

    GeometrySection* GeometryBuilder::createNewSection(quint16 shaderId, quint32 materialId, const QMatrix4x4 &matrix)
    {
        if ( m_Sections.isEmpty() || currentSection()->attributeCount(GeometrySection::PositionAttribute) > 0 )
        {
            m_Sections.append(new GeometrySection(m_pShaderFunctor, m_pTextureFunctor, shaderId, materialId, matrix));
        }
        
        return currentSection();
    }

    GeometrySection* GeometryBuilder::createNewSection()
    {
        return createNewSection(m_iShaderId, m_iMaterialId, m_matModelToWorld);
    }

    int GeometryBuilder::sectionCount() const
    {
        return m_Sections.count();
    }

    void GeometryBuilder::consolidate(QVector<float> &positions, QVector<float> normals,
                                      QVector<float> &colors, QVector<float> &textureCoordinates,
                                      QVector<quint32> &indices) const
    {
        foreach ( GeometrySection* section, m_Sections )
        {
            section->consolidate(positions, normals, colors, textureCoordinates, indices);
        }
    }

    const QList<GeometrySection*>& GeometryBuilder::sections() const
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
            m_Sections[i]->m_matModelToWorld = m_matModelToWorld;
        }
    }

    quint16 GeometryBuilder::shaderId() const
    {
        return m_iShaderId;
    }

    void GeometryBuilder::setShaderId(quint16 id)
    {
        m_iShaderId = id;
    }

    quint32 GeometryBuilder::materialId() const
    {
        return m_iMaterialId;
    }

    void GeometryBuilder::setMaterialId(quint32 id)
    {
        m_iMaterialId = id;
    }

    bool GeometryBuilder::isEmpty() const
    {
        foreach ( GeometrySection* section, m_Sections )
        {
            if ( !section->isEmpty() )
                return false;
        }

        return true;
    }

    IShaderRetrievalFunctor* GeometryBuilder::shaderFunctor() const
    {
        return m_pShaderFunctor;
    }

    ITextureRetrievalFunctor* GeometryBuilder::textureFunctor() const
    {
        return m_pTextureFunctor;
    }

    GeometrySection* GeometryBuilder::section(int index)
    {
        return m_Sections.at(index);
    }

    const GeometrySection* GeometryBuilder::section(int index) const
    {
        return m_Sections.at(index);
    }
}
