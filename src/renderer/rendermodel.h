#ifndef RENDERMODEL_H
#define RENDERMODEL_H

#include "renderer_global.h"
#include <QMultiHash>
#include "rendermodelbatchkey.h"
#include "rendermodelbatch.h"
#include "rendermodelbatchparams.h"

class QOpenGLTexture;

namespace NS_RENDERER
{
    class OpenGLShaderProgram;

    class RENDERERSHARED_EXPORT RenderModel
    {
    public:
        RenderModel();
        ~RenderModel();

        void addItem(const RenderModelBatchKey &key, const RenderModelBatchParams &params,
                     QOpenGLBuffer::UsagePattern usagePattern = QOpenGLBuffer::DynamicDraw);
        void clear();

        void debugDraw(QOpenGLTexture* texture);
        void debugUploadAll();

        // Replace me once we've sorted out shader storage!
        QOpenGLShaderProgram* shaderProgram;
        IShaderSpec* shaderSpec;

    private:
        typedef QMultiHash<RenderModelBatchKey, RenderModelBatch*> RenderModelTable;

        RenderModelTable    m_Table;
    };
}

#endif // RENDERMODEL_H
