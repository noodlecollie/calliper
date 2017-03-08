#ifndef SHADERSTORE_H
#define SHADERSTORE_H

#include "model_global.h"
#include "renderer/opengl/openglshaderprogram.h"
#include "renderer/functors/ishaderretrievalfunctor.h"
#include "model/shaders/knownshaderdefs.h"

namespace Model
{
    class MODELSHARED_EXPORT ShaderStore : public Renderer::IShaderRetrievalFunctor
    {
    public:
        ShaderStore();
        ~ShaderStore();

        virtual Renderer::OpenGLShaderProgram* operator ()(quint16 shaderId) const override;
        Renderer::OpenGLShaderProgram* shaderProgram(KnownShaderDefs::KnownShaderId shaderId) const;

    private:
        void addShaderProgram(Renderer::OpenGLShaderProgram* program);

        Renderer::OpenGLShaderProgram* m_Shaders[KnownShaderDefs::TOTAL_SHADERS];
    };
}

#endif // SHADERSTORE_H
