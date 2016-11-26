#include "vtfheader.h"

namespace FileFormats
{
    namespace
    {
        const char VTF_SIGNATURE[] = { 'V', 'T', 'F', '\0' };
    }

    struct VTFHeader::Data
    {
        // All versions
        char signature[4];
        quint32 version[2];
        quint32 headerSize;
        quint16 width;
        quint16 height;
        quint32 flags;
        quint16 frames;
        quint16 firstFrame;
        char padding0[4];
        float reflectivity[3];
        char padding1[4];
        float bumpMapScale;
        quint32 highResImageFormat;
        quint8 mipmapCount;
        quint32 lowResImageFormat;
        quint8 lowResImageWidth;
        quint8 lowResImageHeight;

        // Version 7.2+
        quint16 depth;

        // Version 7.3+
        char padding2[3];
        quint32 numResources;
    };

    VTFHeader::VTFHeader()
        : StreamDataContainer(),
          m_pData(new VTFHeader::Data())
    {
        memset(m_pData, 0, sizeof(VTFHeader::Data));
    }

    VTFHeader::~VTFHeader()
    {
        delete m_pData;
    }

    bool VTFHeader::populate(QDataStream &stream, QString *errorHint)
    {
        VTFHeader::Data& d = *m_pData;

        beginRead(stream);

        stream.readRawData(d.signature, 4 * sizeof(char));
        stream >> d.version[0] >> d.version[1];

        stream >> d.headerSize
                >> d.width
                >> d.height
                >> d.flags
                >> d.frames
                >> d.firstFrame;

        stream.readRawData(d.padding0, 4 * sizeof(char));
        stream >> d.reflectivity[0]
                >> d.reflectivity[1]
                >> d.reflectivity[2];
        stream.readRawData(d.padding1, 4 * sizeof(char));

        stream >> d.bumpMapScale
                >> d.highResImageFormat
                >> d.mipmapCount
                >> d.lowResImageFormat
                >> d.lowResImageWidth
                >> d.lowResImageHeight;

        if ( (d.version[0] == 7 && d.version[1] >= 2)
             || d.version[0] > 7 )
        {
            stream >> d.depth;
        }

        if ( (d.version[0] == 7 && d.version[1] >= 3)
             || d.version[0] > 7 )
        {
            stream.readRawData(d.padding2, 3 * sizeof(char));
            stream >> d.numResources;
        }

        if ( !endRead(stream, errorHint) )
            return false;

        if ( memcmp(d.signature, VTF_SIGNATURE, 4 * sizeof(char)) != 0 )
        {
            QByteArray ba(d.signature, 4 * sizeof(char));
            ba.append('\0');
            setErrorString(errorHint, QString("Invalid VTF signature '%1'.")
                           .arg(ba.constData()));

            return false;
        }

        return true;
    }

    QString VTFHeader::containerName() const
    {
        return "VTF Header";
    }
}
