#ifndef VTFHEADER_H
#define VTFHEADER_H

#include "file-formats_global.h"
#include "common/streamdatacontainer.h"
#include <QDataStream>

namespace FileFormats
{
    class FILEFORMATSSHARED_EXPORT VTFHeader : public StreamDataContainer
    {
    public:
        VTFHeader();
        ~VTFHeader();

        virtual bool populate(QDataStream& stream, QString* errorHint = nullptr) override;

    protected:
        virtual QString containerName() const override;

    private:
        struct Data;
        Data* m_pData;
    };
}

#endif // VTFHEADER_H
