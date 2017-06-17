#ifndef GEOMETRYRENDERER_H
#define GEOMETRYRENDERER_H

#include <QException>

#include "geometryoffsettable.h"
#include "uniformbatchtable.h"
#include "openglbuffercollection.h"

#include "rendersystem/private/rendermodel/rendermodelcontext.h"
#include "rendersystem/private/shaders/common/privateshaderdefs.h"
#include "rendersystem/interface-classes/definitions/materialdefs.h"
#include "rendersystem/interface-classes/geometry/geometrysection.h"

class OpenGLShaderProgram;

class GeometryRenderer
{
public:
    GeometryRenderer(const RenderModelContext& context,
                     RenderSystem::MaterialDefs::MaterialId materialId,
                     const GeometryOffsetTable& offsets,
                     const UniformBatchTable& batchTable,
                     OpenGLBufferCollection& openGLBuffers);

    void draw();

private:
    class InternalException : public QException
    {
    public:
        const QString error;

        InternalException(const QString& errorString)
            : error(errorString)
        {
        }

        void raise() const
        {
            throw *this;
        }

        InternalException *clone() const
        {
            return new InternalException(*this);
        }
    };

    void getOpenGLUniformBufferAttributes();
    int getLastItemForNextDraw(int firstItem) const;

    void draw_x(int batch);
    void bindBuffers_x(int batch);
    void releaseBuffers();

    const RenderModelContext& m_Context;
    const RenderSystem::MaterialDefs::MaterialId m_nMaterialId;
    const GeometryOffsetTable& m_OffsetTable;
    const UniformBatchTable& m_BatchTable;
    OpenGLBufferCollection& m_OpenGLBuffers;

    int m_nItemsPerBatch;
    OpenGLShaderProgram* m_pCurrentShader;
    RenderSystem::GeometrySection::DrawMode m_nDrawMode;
    float m_flLineWidth;

};

#endif // GEOMETRYRENDERER_H
