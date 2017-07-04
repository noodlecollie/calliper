#ifndef STD140UNIFORMELEMENTBASE_H
#define STD140UNIFORMELEMENTBASE_H

#include <QtGlobal>
#include <QDataStream>

struct Std140UniformStruct;

struct Std140UniformElementBase
{
    Std140UniformElementBase(Std140UniformStruct& parentStruct);

    virtual QDataStream& serialiseToStream(QDataStream& stream) const = 0;
    virtual quint16 baseAlignment() const = 0;

protected:
    Std140UniformStruct& m_ParentStruct;

    void registerElementInParent();
    void writePadding(QDataStream& stream, quint32 numPaddingBytes);
};

QDataStream& operator <<(QDataStream& stream, const Std140UniformElementBase& element);

#endif // STD140UNIFORMELEMENTBASE_H
