#ifndef MATERIALSTORE_H
#define MATERIALSTORE_H

#include "model_global.h"
#include <QHash>
#include "renderer/materials/rendermaterial.h"
#include "renderer/functors/imaterialretrievalfunctor.h"

namespace Model
{
    class MODELSHARED_EXPORT MaterialStore : public Renderer::IMaterialRetrievalFunctor
    {
    public:
        enum PresetMaterial
        {
            UnlitPerVertexColor3D,
        };

        MaterialStore();
        ~MaterialStore();

        virtual Renderer::RenderMaterialPointer operator ()(quint32 materialId) const override;
        Renderer::RenderMaterialPointer getMaterial(quint32 materialId) const;
        Renderer::RenderMaterialPointer createMaterial(const QString &path);
        quint32 getMaterialId(const QString &path) const;
        Renderer::RenderMaterialPointer getMaterial(const QString &path) const;

        Renderer::RenderMaterialPointer defaultMaterial() const;
        Renderer::RenderMaterialPointer presetMaterial(PresetMaterial material) const;
        quint32 presetMaterialId(PresetMaterial material) const;

    private:
        void addPresetMaterials();
        quint32 acquireNextMaterialId();
        void createPresetMaterial(PresetMaterial preset, Renderer::RenderMaterial* material);
        Renderer::RenderMaterialPointer createMaterialInternal(const QString &path, quint32 id);
        Renderer::RenderMaterialPointer createMaterialInternal(Renderer::RenderMaterial* material);

        quint32 m_iNextMaterialId;
        Renderer::RenderMaterialPointer m_pDefaultMaterial;
        QHash<quint32, Renderer::RenderMaterialPointer> m_MaterialTable;
        QHash<QString, quint32> m_MaterialPathTable;
        QHash<PresetMaterial, quint32> m_PresetMaterialTable;
    };
}

#endif // MATERIALSTORE_H
