#ifndef GEOMETRYUPLOADER_H
#define GEOMETRYUPLOADER_H

#include <QSharedPointer>

#include "openglbuffercollection.h"
#include "geometrydatacontainer.h"

class GeometryUploader
{
public:
    GeometryUploader(GeometryDataContainer& data, OpenGLBufferCollection& buffers);

private:
    GeometryDataContainer& m_GeometryDataContainer;
    OpenGLBufferCollection& m_OpenGLBuffers;
};

#endif // GEOMETRYUPLOADER_H
