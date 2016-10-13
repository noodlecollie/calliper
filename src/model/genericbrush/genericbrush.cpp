#include "genericbrush.h"

namespace NS_MODEL
{
    GenericBrush::GenericBrush(const SceneObjectInitParams &initParams, SceneObject* parentObject)
        : SceneObject(initParams, parentObject)
    {
        commonInit();
    }

    GenericBrush::GenericBrush(const GenericBrush* cloneFrom, const SceneObjectInitParams &initParams)
        : SceneObject(cloneFrom, initParams)
    {
        commonInit();
    }

    GenericBrush::~GenericBrush()
    {

    }

    void GenericBrush::bakeGeometry(NS_RENDERER::GeometryBuilder &builder) const
    {

    }

    void GenericBrush::commonInit()
    {

    }

    QVector3D GenericBrush::brushVertexAt(int index) const
    {
        if ( index < 0 || index >= m_BrushVertices.count() )
        {
            Q_ASSERT_X(false, Q_FUNC_INFO, "Index out of bounds!");
            return QVector3D();
        }

        return m_BrushVertices.at(index);
    }

    int GenericBrush::appendBrushVertex(const QVector3D &v)
    {
        m_BrushVertices.append(v);
        return m_BrushVertices.count() - 1;
    }

    void GenericBrush::appendBrushVertices(const QVector<QVector3D> &verts)
    {
        m_BrushVertices.append(verts);
    }

    int GenericBrush::brushVertexCount() const
    {
        return m_BrushVertices.count();
    }

    void GenericBrush::removeBrushVertex(int index)
    {
        if ( index < 0 || index >= m_BrushVertices.count() )
        {
            Q_ASSERT_X(false, Q_FUNC_INFO, "Index out of bounds!");
            return;
        }

        m_BrushVertices.removeAt(index);
    }

    const QVector<QVector3D>& GenericBrush::brushVertexList() const
    {
        return m_BrushVertices;
    }

    void GenericBrush::clearBrushVertices()
    {
        m_BrushVertices.clear();
    }

    QVector<QVector3D> GenericBrush::brushVertexList(const QVector<int> &indices) const
    {
        QVector<QVector3D> verts;

        foreach ( int i, indices )
        {
            verts.append(m_BrushVertices.at(i));
        }

        return verts;
    }

    void GenericBrush::replaceBrushVertex(int index, const QVector3D &v)
    {
        if ( index < 0 || index >= m_BrushVertices.count() )
        {
            Q_ASSERT_X(false, Q_FUNC_INFO, "Index out of bounds!");
            return;
        }

        m_BrushVertices.replace(index, v);
    }

    GenericBrushFacePointer GenericBrush::brushFaceAt(int index) const
    {
        if ( index < 0 || index >= m_BrushFaces.count() )
        {
            Q_ASSERT_X(false, Q_FUNC_INFO, "Index out of bounds!");
            return GenericBrushFacePointer();
        }

        return m_BrushFaces.at(index);
    }

    int GenericBrush::createBrushFace()
    {
        m_BrushFaces.append(GenericBrushFacePointer::create(this));
        return m_BrushFaces.count() - 1;
    }

    void GenericBrush::destroyBrushFace(int index)
    {
        if ( index < 0 || index >= m_BrushFaces.count() )
        {
            Q_ASSERT_X(false, Q_FUNC_INFO, "Index out of bounds!");
            return;
        }

        m_BrushFaces.remove(index);
    }

    void GenericBrush::clearBrushFaces()
    {
        m_BrushFaces.clear();
    }

    const QVector<GenericBrushFacePointer>& GenericBrush::brushFaceList() const
    {
        return m_BrushFaces;
    }

    int GenericBrush::brushFaceCount() const
    {
        return m_BrushFaces.count();
    }
}
