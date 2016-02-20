#ifndef ISERIALISABLE_H
#define ISERIALISABLE_H

#include <QJsonObject>
#include <QString>
#include <QtDebug>

class ISerialisable
{
public:
    virtual ~ISerialisable() {}

    static inline unsigned short SERIALISATION_VERSION() { return 1; }

    static inline QString KEY_IDENTIFIER() { return "identifier"; }
    static inline QString KEY_SUPERCLASS() { return "superclass"; }

    // Return true if serialisable, false otherwise.
    // Eg. Some derived classes will also inherit this
    // interface but shouldn't be saved to files, like
    // the origin marker.
    // If this returns false, the identifier string is undefined.
    virtual bool serialiseToJson(QJsonObject &obj) const = 0;
    virtual QString serialiseIdentifier() const = 0;

protected:
    // Convenience functions for later validation
    static bool validateIdentifier(const QJsonObject &serialisedData, const QString &identifier)
    {
        QJsonValue id = serialisedData.value(ISerialisable::KEY_IDENTIFIER());
        if ( !id.isString() || id.toString() != identifier )
        {
            qWarning() << "WARNING: Serialisation identifier" << (id.isString() ? id.toString() : QString("(null)"))
                       << "does not match" << identifier << "upon unserialisation.";
            Q_ASSERT(false);
            return false;
        }

        return true;
    }
};

#endif // ISERIALISABLE_H

