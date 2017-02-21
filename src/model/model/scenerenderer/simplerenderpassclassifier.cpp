#include "simplerenderpassclassifier.h"

namespace Model
{
    SimpleRenderPassClassifier::SimpleRenderPassClassifier()
    {

    }

    int SimpleRenderPassClassifier::classify(quint32) const
    {
        // One pass only.
        return 0;
    }
}
