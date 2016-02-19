#ifndef ISERIALISABLE_H
#define ISERIALISABLE_H

#include <QJsonObject>
#include <QString>

class ISerialisable
{
public:
    virtual ~ISerialisable() {}

    static inline unsigned short SERIALISATION_VERSION() { return 1; }

    static inline QString KEY_IDENTIFIER() { return "identifier"; }
    static inline QString KEY_SUPERCLASS() { return "superclass"; }

    virtual void serialiseToJson(QJsonObject &obj) const = 0;
    virtual QString serialiseIdentifier() const = 0;
};

#endif // ISERIALISABLE_H

