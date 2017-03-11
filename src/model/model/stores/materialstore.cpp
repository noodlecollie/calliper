#include "materialstore.h"
#include "model/presetmaterials/unlitpervertexcolourpresetmaterial.h"

namespace
{
    const QString PRESET_MATERIAL_PATH_PREFIX = "_presetMaterials";
}

namespace Model
{
    MaterialStore::MaterialStore()
        : m_iNextMaterialId(1),
          m_pDefaultMaterial(Renderer::RenderMaterialPointer::create(0, QString()))
    {
        m_pDefaultMaterial->setShaderTechnique(Renderer::ShaderDefs::LitTextured3D);
        m_pDefaultMaterial->addTexture(Renderer::ShaderDefs::MainTexture, 0);
        addPresetMaterials();
    }

    MaterialStore::~MaterialStore()
    {

    }

    void MaterialStore::addPresetMaterials()
    {
        createPresetMaterial(UnlitPerVertexColor3D,
                             new UnlitPerVertexColourPresetMaterial(acquireNextMaterialId(),
                                                                    PRESET_MATERIAL_PATH_PREFIX + "/unlitpervertexcolor3d"));
    }

    void MaterialStore::createPresetMaterial(PresetMaterial preset, Renderer::RenderMaterial *material)
    {
        createMaterialInternal(material);
        m_PresetMaterialTable.insert(preset, material->materialStoreId());
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
        return createMaterialInternal(new Renderer::RenderMaterial(id, path));
    }

    Renderer::RenderMaterialPointer MaterialStore::createMaterialInternal(Renderer::RenderMaterial *material)
    {
        Renderer::RenderMaterialPointer materialPointer(material);
        m_MaterialTable.insert(materialPointer->materialStoreId(), materialPointer);
        m_MaterialPathTable.insert(materialPointer->path(), materialPointer->materialStoreId());
        return materialPointer;
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

    Renderer::RenderMaterialPointer MaterialStore::presetMaterial(PresetMaterial material) const
    {
        quint32 materialId = m_PresetMaterialTable.value(material, 0);
        if ( materialId == 0 )
        {
            return defaultMaterial();
        }

        return getMaterial(materialId);
    }

    quint32 MaterialStore::presetMaterialId(PresetMaterial material) const
    {
        return m_PresetMaterialTable.value(material, 0);
    }
}
