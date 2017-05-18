#ifndef GEOMETRYUPLOADER_H
#define GEOMETRYUPLOADER_H

#include <QSharedPointer>

#include "openglbuffercollection.h"
#include "geometrydatacontainer.h"

#include "rendersystem/private/shaders/common/privateshaderdefs.h"

class GeometryUploader
{
public:
    GeometryUploader(GeometryDataContainer& data, OpenGLBufferCollection& buffers);

    void uploadIfRequired(PrivateShaderDefs::ShaderId shaderId);

private:
    enum UploadFlags
    {
        NoUploadFlags = 0,

        MatricesUploadFlag = (1<<0),
        VerticesUploadFlag = (1<<1),

        AllUploadFlags = MatricesUploadFlag | VerticesUploadFlag
    };

    quint32 shouldUpload(PrivateShaderDefs::ShaderId shaderId) const;
    quint32 checkForDirtyGeometry() const;

    GeometryDataContainer& m_GeometryDataContainer;
    OpenGLBufferCollection& m_OpenGLBuffers;
};

#endif // GEOMETRYUPLOADER_H
