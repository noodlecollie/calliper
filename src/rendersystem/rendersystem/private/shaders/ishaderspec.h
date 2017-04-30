#ifndef ISHADERSPEC_H
#define ISHADERSPEC_H

#include "vertexformat.h"

class IShaderSpec
{
public:
    virtual ~IShaderSpec() {}

    virtual VertexFormat vertexFormat() const = 0;

    // Maximum number of items supported in a batch.
    // Return 1 if the shader doesn't support batching.
    virtual int maxBatchedItems() const = 0;
};

#endif // ISHADERSPEC_H
