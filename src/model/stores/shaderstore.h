#ifndef SHADERSTORE_H
#define SHADERSTORE_H

#include "model_global.h"
#include <QHash>
#include "opengl/openglshaderprogram.h"
#include "functors/ishaderretrievalfunctor.h"
#include <QHash>

namespace Model
{
    class MODELSHARED_EXPORT ShaderStore : public Renderer::IShaderRetrievalFunctor
    {
    public:
        ShaderStore();
        ~ShaderStore();

        virtual Renderer::OpenGLShaderProgram* operator ()(quint16 shaderId) const override;
        Renderer::OpenGLShaderProgram* getShaderProgram(quint16 shaderId) const;

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
        Renderer::OpenGLShaderProgram* getShaderProgram(const QString &shaderName) const;

    private:
        quint16 acquireNextShaderId();

        quint16 m_iNextShaderId;
        QHash<quint16, Renderer::OpenGLShaderProgram*> m_ShaderTable;
    };
}

#endif // SHADERSTORE_H
