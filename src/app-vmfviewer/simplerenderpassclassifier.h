#ifndef SIMPLERENDERPASSCLASSIFIER_H
#define SIMPLERENDERPASSCLASSIFIER_H

#include "model/scenerenderer/irenderpassclassifier.h"

class SimpleRenderPassClassifier : public Model::IRenderPassClassifier
{
public:
    SimpleRenderPassClassifier();

    virtual int classify(quint32 objectId) const override;
};

#endif // SIMPLERENDERPASSCLASSIFIER_H
