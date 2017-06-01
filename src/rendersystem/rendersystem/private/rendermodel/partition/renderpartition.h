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

#include "rendersystem/private/rendermodel/rendermodelcontext.h"
#include "rendersystem/interface-classes/geometry/geometrysection.h"

class RenderPartition
{
public:
    RenderPartition(const RenderModelContext& context,
                    RenderSystem::MaterialDefs::MaterialId materialId,
                    int maxItems);

    int maxItems() const;
    int itemCount() const;
    bool isFull() const;
    int freeSpaces() const;

    void setGeometry(const QSharedPointer<RenderSystem::GeometrySection>& section);
    void removeGeometry(RenderSystem::PublicRenderModelDefs::ObjectId objectId);

    void draw(GLenum drawMode, float lineWidth);

private:
    const RenderModelContext& m_Context;
    const RenderSystem::MaterialDefs::MaterialId m_nMaterialId;
    const int m_nMaxItems;

    // Data
    GeometryDataContainer m_GeometryDataContainer;
    OpenGLBufferCollection m_OpenGLBuffers;
    GeometryOffsetTable m_OffsetTable;

    // Operators on data
    GeometryUploader m_Uploader;
};

#endif // RENDERPARTITION_H
