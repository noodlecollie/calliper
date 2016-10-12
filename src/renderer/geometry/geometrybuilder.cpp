#include "geometrybuilder.h"
#include <QVector4D>

namespace NS_RENDERER
{
    GeometryBuilder::GeometryBuilder(IShaderRetrievalFunctor* shaderFunctor, ITextureRetrievalFunctor* textureFunctor,
                                     quint16 shaderId, quint32 textureId, const QMatrix4x4 &modelToWorldMatrix)
        : m_pShaderFunctor(shaderFunctor), m_pTextureFunctor(textureFunctor),
          m_iShaderId(shaderId), m_iTextureId(textureId), m_matModelToWorld(modelToWorldMatrix)
    {
        createNewSection();
    }

    GeometryBuilder::~GeometryBuilder()
    {
        qDeleteAll(m_Sections);
    }

    GeometrySection* GeometryBuilder::currentSection()
    {
       return m_Sections.last();
    }

    GeometrySection* GeometryBuilder::createNewSection(quint16 shaderId, quint32 textureId, const QMatrix4x4 &matrix)
    {
        m_Sections.append(new GeometrySection(m_pShaderFunctor, m_pTextureFunctor, shaderId, textureId, matrix));
        return currentSection();
    }

    GeometrySection* GeometryBuilder::createNewSection()
    {
        m_Sections.append(new GeometrySection(m_pShaderFunctor, m_pTextureFunctor, m_iShaderId, m_iTextureId, m_matModelToWorld));
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

    quint32 GeometryBuilder::textureId() const
    {
        return m_iTextureId;
    }

    void GeometryBuilder::setTextureId(quint32 id)
    {
        m_iTextureId = id;
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
}
