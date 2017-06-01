#ifndef RENDERUTILS_H
#define RENDERUTILS_H

#include <QSharedPointer>

#include "rendersystem/private/shaders/base/openglshaderprogram.h"
#include "rendersystem/interface-classes/definitions/materialdefs.h"

namespace RenderUtils
{
    OpenGLShaderProgram* shaderFromMaterial(RenderSystem::PublicShaderDefs::RenderMode renderMode,
                                            RenderSystem::MaterialDefs::MaterialId materialId);
}

#endif // RENDERUTILS_H
