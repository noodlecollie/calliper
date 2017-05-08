#ifndef RENDERBATCH_H
#define RENDERBATCH_H

#include <QMultiHash>
#include "renderbatchpartition.h"

class RenderBatch
{
public:
    RenderBatch();

private:
    QMultiHash<quint32, RenderBatchPartition> m_Partitions;
};

#endif // RENDERBATCH_H
