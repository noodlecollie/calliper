#include "std140uniformstruct.h"
#include "std140uniformelementbase.h"

QDataStream& operator <<(QDataStream& stream, const Std140UniformStruct& item)
{
    return item.serialise(stream);
}

QDataStream& Std140UniformStruct::serialise(QDataStream &stream) const
{
    for ( const Std140UniformElementBase* const element : m_Elements )
    {
        stream << (*element);
    }

    return stream;
}

quint16 Std140UniformStruct::baseAlignment() const
{
    quint16 alignment = 0;

    for ( const Std140UniformElementBase* const element : m_Elements )
    {
        const quint16 elementAlignment = element->baseAlignment();
        if ( elementAlignment > alignment )
        {
            alignment = elementAlignment;
        }
    }

    return alignment;
}
