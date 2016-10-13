#ifndef SIMPLERENDERPASSCLASSIFIER_H
#define SIMPLERENDERPASSCLASSIFIER_H

#include "scenerenderer/irenderpassclassifier.h"

class SimpleRenderPassClassifier : public NS_MODEL::IRenderPassClassifier
{
public:
    SimpleRenderPassClassifier();

    virtual int classify(quint32 objectId) const override;
};

#endif // SIMPLERENDERPASSCLASSIFIER_H
