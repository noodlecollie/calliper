#include "simplerenderpassclassifier.h"

SimpleRenderPassClassifier::SimpleRenderPassClassifier()
{

}

int SimpleRenderPassClassifier::classify(quint32) const
{
    // Everything goes in one pass.
    return 0;
}
