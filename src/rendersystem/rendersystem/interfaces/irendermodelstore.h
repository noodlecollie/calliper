#ifndef IRENDERMODELSTORE_H
#define IRENDERMODELSTORE_H

#include "rendersystem_global.h"

#include <QMatrix4x4>

#include "rendersystem/interface-classes/definitions/rendermodeldefs.h"
#include "rendersystem/interface-classes/geometry/geometrybuilder.h"
#include "rendersystem/interface-classes/definitions/framebufferdefs.h"
#include "rendersystem/interface-classes/definitions/shaderdefs.h"
#include "rendersystem/interface-classes/drawing/framedrawparams.h"

namespace RenderSystem
{
    class IRenderModelStore
    {
    public:
        virtual ~IRenderModelStore() {}

        // Creates a new, blank render model and returns its ID.
        virtual RenderModelDefs::RenderModelId createRenderModel() = 0;

        // Destroys an existing render model.
        virtual void removeRenderModel(RenderModelDefs::RenderModelId modelId) = 0;

        // Adds geometry to the render model.
        // If the object ID of this geometry already exists in the render model,
        // the old geometry is replaced.
        virtual void addGeometry(RenderModelDefs::RenderModelId modelId, const GeometryBuilder& builder) = 0;

        // Removes any geometry that exists in the model for the given object ID.
        virtual void removeGeometry(RenderModelDefs::RenderModelId modelId, RenderModelDefs::ObjectId objectId) = 0;

        // Clears all geometry from the model, but does not destroy the model itself.
        virtual void clearGeometry(RenderModelDefs::RenderModelId modelId) = 0;

        // Draws the given render model into the given frame buffer.
        virtual void draw(const RenderModelDefs::RenderModelId modelId,
                          const FrameBufferDefs::FrameBufferId frameBufferId,
                          const RenderSystem::FrameDrawParams& drawParams) = 0;
    };
}

#endif // IRENDERMODELSTORE_H
