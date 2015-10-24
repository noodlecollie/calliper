#ifndef CBASERENDERER_H
#define CBASERENDERER_H

class CBaseRenderer
{
public:
    CBaseRenderer();

    // Formats for vertex buffers.
    // Attributes are interleaved in the order specified.
    enum InterleavingFormat
    {
        FormatPosition = 0,
        FormatPositionUV
    };

    // Attribute types.
    // The value within this enum corresponds to the expected attribute
    // number used in the shader.
    enum Attribute
    {
        Position     = 0,
        UV          // 1
    };

    // These are all IN BYTES.
    // Offset from the beginning of the buffer for the first instance of the given attribute.
    static int attributeOffset(InterleavingFormat format, Attribute att);

    // How large a given attribute is.
    static int attributeSize(Attribute att);

    // How large an entire collection of attributes is.
    // This is the same as the stride required for the format.
    static int interleavingFormatSize(InterleavingFormat format);
};

#endif // CBASERENDERER_H
