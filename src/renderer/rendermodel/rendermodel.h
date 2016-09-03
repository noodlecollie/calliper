#ifndef RENDERMODEL_H
#define RENDERMODEL_H

#include "renderer_global.h"
#include <QMultiHash>
#include "rendermodelbatchkey.h"
#include "rendermodelbatch.h"
#include "rendermodelbatchparams.h"
#include <QMap>

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

    private:
        typedef QMultiHash<RenderModelBatchKey, RenderModelBatch*> RenderModelTable;
        typedef QMap<RenderModelBatchKey, char> KeyOrderingMap;

        RenderModelTable    m_Table;
        KeyOrderingMap      m_KeyOrdering;
    };
}

#endif // RENDERMODEL_H
