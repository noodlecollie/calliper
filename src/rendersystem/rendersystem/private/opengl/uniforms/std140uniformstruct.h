#ifndef STD140UNIFORMSTRUCT_H
#define STD140UNIFORMSTRUCT_H

#include <QVector>
#include <QDataStream>

struct Std140UniformElementBase;

struct Std140UniformStruct
{
    friend struct Std140UniformElementBase;

    QDataStream& serialise(QDataStream& stream) const;

    quint16 baseAlignment() const;

private:
    QVector<const Std140UniformElementBase*> m_Elements;
};

QDataStream& operator <<(QDataStream& stream, const Std140UniformStruct& item);

#endif // STD140UNIFORMSTRUCT_H
