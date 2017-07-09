#ifndef RENDERPARTITION_H
#define RENDERPARTITION_H

#include <QOpenGLBuffer>
#include <QSharedPointer>

#include "geometrydatakey.h"
#include "geometrydata.h"
#include "openglbuffercollection.h"
#include "geometryuploader.h"
#include "geometrydatacontainer.h"
#include "geometryoffsettable.h"
#include "uniformbatchtable.h"

#include "rendersystem/private/rendermodel/rendermodelcontext.h"
#include "rendersystem/interface-classes/geometry/geometrysection.h"

/*
 * The render partition is one section of a render group, and encapsulates a set of buffers
 * that are used to render objects that use the given material. This means that, in an ideal
 * situation, we can make one call to glDrawElements() and have all the objects in the
 * partition drawn by this call. However, this might not always be the case - different
 * objects could, for example, have different line widths or draw modes, and at a later date
 * we'll need to think about the best way to address this. Currently these objects are just
 * drawn by using glBindBufferRange() and by drawing what's in the buffers one section at a
 * time.
 *
 * The general life cycle should be as follows:
 *
 * === Initialisation ===
 * Create VAO, bind.
 * Create buffer collection (vertex, index, uniform buffers).
 * Set up vertex attributes (this may need to be re-done if the
 *      shader referenced by a material changes).
 * Release VAO
 *
 * === Uploading ===
 * TODO
 */
class RenderPartition
{
public:
    RenderPartition(const RenderModelContext& context,
                    RenderSystem::MaterialDefs::MaterialId materialId,
                    int maxItems);
    ~RenderPartition();

    int maxItems() const;
    int itemCount() const;
    bool isFull() const;
    int freeSpaces() const;

    void setGeometry(const QSharedPointer<RenderSystem::GeometrySection>& section);
    void removeGeometry(RenderSystem::RenderModelDefs::ObjectId objectId);

    void draw();

private:
    const RenderModelContext& m_Context;
    const RenderSystem::MaterialDefs::MaterialId m_nMaterialId;
    const int m_nMaxItems;

    // Data
    GeometryDataContainer m_GeometryDataContainer;
    OpenGLBufferCollection m_OpenGLBuffers;
    GeometryOffsetTable m_OffsetTable;
    UniformBatchTable m_BatchTable;

    // Operators on data
    GeometryUploader m_Uploader;
};

#endif // RENDERPARTITION_H
