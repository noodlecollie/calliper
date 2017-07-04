#include "std140uniformelementbase.h"

#include <QByteArray>

#include "std140uniformstruct.h"

QDataStream& operator <<(QDataStream& stream, const Std140UniformElementBase& element)
{
    return element.serialiseToStream(stream);
}

Std140UniformElementBase::Std140UniformElementBase(Std140UniformStruct &parentStruct)
    : m_ParentStruct(parentStruct)
{
}

void Std140UniformElementBase::registerElementInParent()
{
    m_ParentStruct.m_Elements.append(this);
}

void Std140UniformElementBase::writePadding(QDataStream &stream, quint32 numPaddingBytes)
{
    QByteArray padding(numPaddingBytes, 0);
    stream.writeBytes(padding.constData(), numPaddingBytes);
}
