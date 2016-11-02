#include "genericbrushface.h"
#include "genericbrush.h"
#include "scene/scene.h"

namespace NS_MODEL
{
    GenericBrushFace::GenericBrushFace(GenericBrush* parentBrush)
        : QObject(parentBrush), m_pTexturePlane(new TexturePlane(this))
    {
        Q_ASSERT_X(parentBrush, Q_FUNC_INFO, "Parent brush cannot be null!");

        connect(m_pTexturePlane, &TexturePlane::dataChanged, this, &GenericBrushFace::dataChanged);
    }

    GenericBrush* GenericBrushFace::parentBrush() const
    {
        return qobject_cast<GenericBrush*>(parent());
    }

    int GenericBrushFace::indexAt(int index) const
    {
        if ( index < 0 || index >= m_BrushVertexIndices.count() )
        {
            Q_ASSERT_X(false, Q_FUNC_INFO, "Index out of bounds!");
            return -1;
        }

        return m_BrushVertexIndices.at(index);
    }

    void GenericBrushFace::appendIndex(int i)
    {
        m_BrushVertexIndices.append(i);
        emit dataChanged();
    }

    void GenericBrushFace::appendIndices(const QVector<int> &indices)
    {
        m_BrushVertexIndices.reserve(m_BrushVertexIndices.count() + indices.count());

        for ( int i = 0; i < indices.count(); i++ )
        {
            m_BrushVertexIndices.append(indices.at(i));
        }

        emit dataChanged();
    }

    int GenericBrushFace::indexCount() const
    {
        return m_BrushVertexIndices.count();
    }

    void GenericBrushFace::removeIndex(int index)
    {
        if ( index < 0 || index >= m_BrushVertexIndices.count() )
        {
            Q_ASSERT_X(false, Q_FUNC_INFO, "Index out of bounds!");
            return;
        }

        m_BrushVertexIndices.remove(index);
        emit dataChanged();
    }

    QVector<int> GenericBrushFace::indexList() const
    {
        return m_BrushVertexIndices;
    }

    void GenericBrushFace::clearIndices()
    {
        m_BrushVertexIndices.clear();
        emit dataChanged();
    }

    QVector<QVector3D> GenericBrushFace::referencedBrushVertexList() const
    {
        return parentBrush()->brushVertexList(m_BrushVertexIndices);
    }

    void GenericBrushFace::buildFaceGeometry(const ShaderPalette&, Renderer::GeometryBuilder &builder) const
    {
        using namespace Renderer;

        GeometrySection* section = builder.createNewSection(
                    builder.shaderId(),
                    texturePlane()->textureId(),
                    builder.modelToWorldMatrix());

        QVector<QVector3D> vertices = referencedBrushVertexList();

        section->addPositions(vertices);

        for ( int i = 2; i < vertices.count(); ++i )
        {
            section->addIndexTriangle(0, i-1, i);
        }

        QVector3D nrm = normal();
        for ( int i = 0; i < indexCount(); i++ )
        {
            section->addNormal(nrm);
        }

        TextureStore* texStore = parentBrush()->parentScene()->textureStore();
        OpenGLTexturePointer tex = (*texStore)(m_pTexturePlane->textureId());
        for ( int i = 0; i < vertices.count(); i++ )
        {
            section->addTextureCoordinate(m_pTexturePlane->textureCoordinate(vertices.at(i), tex->size(), nrm));
        }
    }

    QVector3D GenericBrushFace::normal() const
    {
        if ( indexCount() < 3 )
            return QVector3D();

        QVector3D v0 = parentBrush()->brushVertexAt(m_BrushVertexIndices.at(0));
        QVector3D v1 = parentBrush()->brushVertexAt(m_BrushVertexIndices.at(1));
        QVector3D v2 = parentBrush()->brushVertexAt(m_BrushVertexIndices.at(2));

        return QVector3D::normal(v1-v0, v2-v0);
    }

    TexturePlane* GenericBrushFace::texturePlane() const
    {
        return m_pTexturePlane;
    }
}
