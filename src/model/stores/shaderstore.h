#ifndef SHADERSTORE_H
#define SHADERSTORE_H

#include "model_global.h"
#include <QHash>
#include "opengl/openglshaderprogram.h"
#include "functors/ishaderretrievalfunctor.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT ShaderStore : public NS_RENDERER::IShaderRetrievalFunctor
    {
    public:
        ShaderStore();
        ~ShaderStore();

        virtual NS_RENDERER::OpenGLShaderProgram* operator ()(quint16 shaderId) const override;
        NS_RENDERER::OpenGLShaderProgram* getShaderProgram(quint16 shaderId) const;

        template<typename T>
        quint16 addShaderProgram()
        {
            quint16 id = acquireNextShaderId();
            T* shader = new T(id);
            shader->construct();
            m_ShaderTable.insert(id, shader);
            return id;
        }

        quint16 getShaderId(const QString shaderName) const;
        NS_RENDERER::OpenGLShaderProgram* getShaderProgram(const QString &shaderName) const;

    private:
        quint16 acquireNextShaderId();

        quint16 m_iNextShaderId;
        QHash<quint16, NS_RENDERER::OpenGLShaderProgram*> m_ShaderTable;
    };
}

#endif // SHADERSTORE_H
