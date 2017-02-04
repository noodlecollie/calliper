#include "materialstore.h"

namespace Model
{
    MaterialStore::MaterialStore()
        : m_iNextMaterialId(1),
          m_pDefaultMaterial(Renderer::RenderMaterialPointer::create(0, QString()))
    {
        m_pDefaultMaterial->addTexture(Renderer::ShaderDefs::MainTexture, 0);
    }

    MaterialStore::~MaterialStore()
    {

    }

    quint32 MaterialStore::acquireNextMaterialId()
    {
        Q_ASSERT_X(m_iNextMaterialId + 1 != 0, Q_FUNC_INFO, "Material ID counter overflow!");
        return m_iNextMaterialId++;
    }

    Renderer::RenderMaterialPointer MaterialStore::getMaterial(quint32 materialId) const
    {
        return m_MaterialTable.value(materialId, m_pDefaultMaterial);
    }

    Renderer::RenderMaterialPointer MaterialStore::operator ()(quint32 materialId) const
    {
        return getMaterial(materialId);
    }

    Renderer::RenderMaterialPointer MaterialStore::createMaterial(const QString &path)
    {
        using namespace Renderer;

        if ( m_MaterialPathTable.contains(path) )
        {
            return getMaterial(m_MaterialPathTable.value(path));
        }

        return createMaterialInternal(path, acquireNextMaterialId());
    }

    Renderer::RenderMaterialPointer MaterialStore::createMaterialInternal(const QString &path, quint32 id)
    {
        using namespace Renderer;

        RenderMaterialPointer material = RenderMaterialPointer::create(id, path);
        m_MaterialTable.insert(id, material);
        m_MaterialPathTable.insert(path, id);
        return material;
    }

    quint32 MaterialStore::getMaterialId(const QString &path) const
    {
        return m_MaterialPathTable.value(path, 0);
    }

    Renderer::RenderMaterialPointer MaterialStore::getMaterial(const QString &path) const
    {
        return getMaterial(getMaterialId(path));
    }

    Renderer::RenderMaterialPointer MaterialStore::defaultMaterial() const
    {
        return m_pDefaultMaterial;
    }
}
