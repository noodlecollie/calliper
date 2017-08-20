#ifndef IRENDERMODELSTORE_H
#define IRENDERMODELSTORE_H

#include "rendersystem_global.h"

#include <QMatrix4x4>

#include "rendersystem/interface-classes/definitions/rendermodeldefs.h"
#include "rendersystem/interface-classes/geometry/geometrybuilder.h"
#include "rendersystem/interface-classes/definitions/framebufferdefs.h"
#include "rendersystem/interface-classes/definitions/shaderdefs.h"
#include "rendersystem/interface-classes/rendering/framedrawparams.h"

#include "profiling/profilermodel/profileritemmodeladatper.h"

namespace RenderSystem
{
    class IRenderModelStore
    {
    public:
        virtual ~IRenderModelStore() {}

        // Creates a new, blank render model and returns its ID.
        // The name should be descriptive, and is recommended to be unique.
        virtual RenderModelDefs::RenderModelId createRenderModel(const QString& name) = 0;

        // Returns the name of the given render model.
        virtual QString name(const RenderModelDefs::RenderModelId modelId) const = 0;

        // Sets the name of the given render model.
        virtual void setName(const RenderModelDefs::RenderModelId modelId, const QString& newName) const = 0;

        // Destroys an existing render model.
        virtual void removeRenderModel(const RenderModelDefs::RenderModelId modelId) = 0;

        // Adds geometry to the render model.
        // If the object ID of this geometry already exists in the render model,
        // the old geometry is replaced.
        virtual void addGeometry(const RenderModelDefs::RenderModelId modelId, const GeometryBuilder& builder) = 0;

        // Removes any geometry that exists in the model for the given object ID.
        virtual void removeGeometry(const RenderModelDefs::RenderModelId modelId, const RenderModelDefs::ObjectId objectId) = 0;

        // Clears all geometry from the model, but does not destroy the model itself.
        virtual void clearGeometry(const RenderModelDefs::RenderModelId modelId) = 0;

        // Draws the given render model into the given frame buffer.
        virtual void draw(const RenderModelDefs::RenderModelId modelId,
                          const FrameBufferDefs::FrameBufferId frameBufferId,
                          const FrameDrawParams& drawParams) = 0;

        // Returns the given render model's profiling data.
        virtual Profiling::ProfilerItemModelAdatper* profilingData(const RenderModelDefs::RenderModelId modelId) const = 0;

        // Returns the QAbstractItemModel interface for displaying items.
        virtual QAbstractItemModel* itemModel() const = 0;

        // Returns the QAbstractItemModel interface for displaying profiling information of all render models.
        virtual QAbstractItemModel* profilingItemModel() const = 0;
    };
}

#endif // IRENDERMODELSTORE_H
