#ifndef SIMPLERENDERPASSCLASSIFIER_H
#define SIMPLERENDERPASSCLASSIFIER_H

#include "model_global.h"
#include "irenderpassclassifier.h"

namespace Model
{
    class MODELSHARED_EXPORT SimpleRenderPassClassifier : public IRenderPassClassifier
    {
    public:
        SimpleRenderPassClassifier();

        virtual int classify(quint32) const override;
    };
}

#endif // SIMPLERENDERPASSCLASSIFIER_H
