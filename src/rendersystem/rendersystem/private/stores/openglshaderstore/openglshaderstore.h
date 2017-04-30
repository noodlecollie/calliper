#ifndef OPENGLSHADERSTORE_H
#define OPENGLSHADERSTORE_H

#include "rendersystem/private/store-classes/itempointerbasedobjectstore.h"
#include "rendersystem/private/shaders/base/openglshaderprogram.h"

class OpenGLShaderStore : public ItemPointerBasedObjectStore<OpenGLShaderProgram, quint16>
{
protected:
    virtual void objectCreated(const ObjectId id) override;
    virtual void objectAboutToBeDestroyed(const ObjectId id) override;
};

#endif // OPENGLSHADERSTORE_H
