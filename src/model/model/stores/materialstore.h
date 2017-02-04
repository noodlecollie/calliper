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
        MaterialStore();
        ~MaterialStore();

        virtual Renderer::RenderMaterialPointer operator ()(quint32 materialId) const override;
        Renderer::RenderMaterialPointer getMaterial(quint32 materialId) const;
        Renderer::RenderMaterialPointer createMaterial(const QString &path);
        quint32 getMaterialId(const QString &path) const;
        Renderer::RenderMaterialPointer getMaterial(const QString &path) const;

        Renderer::RenderMaterialPointer defaultMaterial() const;

    private:
        quint32 acquireNextMaterialId();
        Renderer::RenderMaterialPointer createMaterialInternal(const QString &path, quint32 id);

        quint32 m_iNextMaterialId;
        Renderer::RenderMaterialPointer m_pDefaultMaterial;
        QHash<quint32, Renderer::RenderMaterialPointer> m_MaterialTable;
        QHash<QString, quint32> m_MaterialPathTable;
    };
}

#endif // MATERIALSTORE_H
