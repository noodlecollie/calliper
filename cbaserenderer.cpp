#include "cbaserenderer.h"

// Size of each attribute in bytes.
static const int ATTRIBUTE_SIZE[] = {
    3 * sizeof(float),      // Position
    2 * sizeof(float),      // UV
};

// Size of each interleaving format in bytes.
static const int FORMAT_SIZE[] = {
    ATTRIBUTE_SIZE[CBaseRenderer::Position],
    ATTRIBUTE_SIZE[CBaseRenderer::Position] + ATTRIBUTE_SIZE[CBaseRenderer::UV],
};

int CBaseRenderer::attributeOffset(InterleavingFormat format, Attribute att)
{
    switch (format)
    {
        case FormatPosition:
        {
            switch (att)
            {
            case Position:
                return 0;
            default:
                return 0;
            }
        }

        case FormatPositionUV:
        {
            switch(att)
            {
            case Position:
                return 0;
            case UV:
                return 3 * sizeof(float);
            default:
                return 0;
            }
        }

    default:
        return 0;
    }
}

int CBaseRenderer::attributeSize(Attribute att)
{
    return ATTRIBUTE_SIZE[att];
}

int CBaseRenderer::interleavingFormatSize(InterleavingFormat format)
{
    return FORMAT_SIZE[format];
}

CBaseRenderer::CBaseRenderer(QObject *parent) : QObject(parent)
{

}
