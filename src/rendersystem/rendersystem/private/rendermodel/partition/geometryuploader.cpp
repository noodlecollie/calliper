#include "geometryuploader.h"

GeometryUploader::GeometryUploader(GeometryDataContainer &data, OpenGLBufferCollection &buffers)
    : m_GeometryDataContainer(data),
      m_OpenGLBuffers(buffers)
{

}
