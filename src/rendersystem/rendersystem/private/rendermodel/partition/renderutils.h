#ifndef RENDERUTILS_H
#define RENDERUTILS_H

#include <QSharedPointer>

#include "rendersystem/private/shaders/base/openglshaderprogram.h"
#include "rendersystem/interface-classes/store/publicstoredefs.h"
#include "rendersystem/interface-classes/shader/publicshaderdefs.h"

namespace RenderUtils
{
    OpenGLShaderProgram* shaderFromMaterial(RenderSystem::PublicShaderDefs::RenderMode renderMode,
                                            RenderSystem::PublicStoreDefs::MaterialId materialId);
}

#endif // RENDERUTILS_H
