#ifndef STD140UNIFORMELEMENT_H
#define STD140UNIFORMELEMENT_H

#include <QDataStream>

#include "std140uniformelementbase.h"
#include "std140serialisation.h"

template<typename T>
struct Std140UniformElement : public Std140UniformElementBase
{
    T data;

    template<typename... ARGS>
    Std140UniformElement(Std140UniformStruct& parentStruct, ARGS... args)
        : Std140UniformElementBase(parentStruct),
          data(std::move(args)...)
    {
        registerElementInParent();
    }

    virtual QDataStream& serialiseToStream(QDataStream& stream) const override
    {
        return UniformStd140::serialise(stream, data);
    }

    virtual quint16 baseAlignment() const override
    {
        return UniformStd140::baseAlignment<T>().baseAlignment;
    }
};

#endif // STD140UNIFORMELEMENT_H
