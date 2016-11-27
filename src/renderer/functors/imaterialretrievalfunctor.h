#ifndef IMATERIALRETRIEVALFUNCTOR_H
#define IMATERIALRETRIEVALFUNCTOR_H

#include "renderer_global.h"
#include <QSharedPointer>
#include "materials/rendermaterial.h"

namespace Renderer
{
    typedef QSharedPointer<RenderMaterial> RenderMaterialPointer;

    class IMaterialRetrievalFunctor
    {
    public:
        virtual ~IMaterialRetrievalFunctor() {}

        // Get a pointer to a material by ID.
        virtual RenderMaterialPointer operator ()(quint32 materialId) const = 0;
    };
}


#endif // IMATERIALRETRIEVALFUNCTOR_H
