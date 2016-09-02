#ifndef SHADERSTORE_H
#define SHADERSTORE_H

#include "renderer_global.h"
#include "openglshaderprogram.h"
#include <QList>
#include <QHash>

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT ShaderStore
    {
    public:
        static ShaderStore* getShaderStore();

        ShaderStore();
        ~ShaderStore();

        void constructShaders();

        OpenGLShaderProgram* shader(const QString &name) const;
        OpenGLShaderProgram* shader(quint16 id) const;
        quint16 shaderId(const QString &name) const;

    private:
        void addBuiltInShaders();
        void addShader(OpenGLShaderProgram* shader);

        QList<OpenGLShaderProgram*> m_ShaderList;
        QHash<QString, quint16>     m_ShaderNameTable;
    };
}

#endif // SHADERSTORE_H
