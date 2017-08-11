#include "originmarker.h"

#include "model/scene/scene.h"

#include "rendersystem/endpoints/presetmaterialstoreendpoint.h"

namespace Model
{
    OriginMarker::OriginMarker(const SceneObjectInitParams &initParams, SceneObject* parentObject)
        : SceneObject(initParams, parentObject)
    {
        commonInit();
    }

    OriginMarker::OriginMarker(const OriginMarker* cloneFrom, const SceneObjectInitParams &initParams)
        : SceneObject(cloneFrom, initParams)
    {
        commonInit();
    }

    OriginMarker::~OriginMarker()
    {

    }

    void OriginMarker::commonInit()
    {
        updateScalableState(scalable());
    }

    void OriginMarker::bakeGeometry(RenderSystem::GeometryBuilder &builder) const
    {
        using namespace RenderSystem;

        const RenderSystem::MaterialDefs::MaterialId materialId = getPresetMaterialId();

        Q_ASSERT(materialId != MaterialDefs::INVALID_MATERIAL_ID);

        QSharedPointer<GeometrySection> section = builder.createNewSection(materialId, builder.modelToWorldMatrix());

        section->setDrawMode(GeometrySection::DrawLines);

        section->addPosition(QVector3D(0,0,0));
        section->addPosition(QVector3D(64,0,0));
        section->addPosition(QVector3D(0,0,0));
        section->addPosition(QVector3D(0,64,0));
        section->addPosition(QVector3D(0,0,0));
        section->addPosition(QVector3D(0,0,64));

        section->addColor(QColor::fromRgb(0xffff0000));
        section->addColor(QColor::fromRgb(0xffff0000));
        section->addColor(QColor::fromRgb(0xff00ff00));
        section->addColor(QColor::fromRgb(0xff00ff00));
        section->addColor(QColor::fromRgb(0xff0000ff));
        section->addColor(QColor::fromRgb(0xff0000ff));

        section->addIndexPair(0,1);
        section->addIndexPair(2,3);
        section->addIndexPair(4,5);
    }

    bool OriginMarker::scalable() const
    {
        return false;
    }

    bool OriginMarker::customVertexColours() const
    {
        return true;
    }

    RenderSystem::MaterialDefs::MaterialId OriginMarker::getPresetMaterialId() const
    {
        return RenderSystem::PresetMaterialStoreEndpoint::presetMaterialStore()
                ->material(RenderSystem::MaterialDefs::UnlitMaterial);
    }
}
