#ifndef IRENDERPASSCLASSIFIER_H
#define IRENDERPASSCLASSIFIER_H

#include "model_global.h"

namespace NS_MODEL
{
    class IRenderPassClassifier
    {
    public:
        virtual ~IRenderPassClassifier() {}

        // Returns a render pass index for a given object.
        virtual int classify(quint32 objectId) const = 0;
    };
}

#endif // IRENDERPASSCLASSIFIER_H
