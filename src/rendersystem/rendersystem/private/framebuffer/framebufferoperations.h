#ifndef FRAMEBUFFEROPERATIONS_H
#define FRAMEBUFFEROPERATIONS_H

#include "rendersystem/interfaces/iframebufferoperations.h"

class FrameBufferOperations : public RenderSystem::IFrameBufferOperations
{
public:
    virtual void saveToFile(const RenderSystem::FrameBufferDefs::FrameBufferId id, const QString &filename) override;
    virtual void drawContentsToCurrent(const RenderSystem::FrameBufferDefs::FrameBufferId id) override;
};

#endif // FRAMEBUFFEROPERATIONS_H
