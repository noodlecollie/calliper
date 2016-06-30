#ifndef KEYVALUESPARSER_H
#define KEYVALUESPARSER_H

#include <QByteArray>

// A basic KV file is -almost- a JSON file. We can treat it as the following:
// - Every KV object (ie. key with subkeys) corresponds to a JSON object.
// - Every KV pair (ie. key with a value) should be treated as a JSON object
//   entry. The value should be treated as a string (as KV values don't have
//   explicit types.

// The following modifications need to be made:
// - KV files allow many root KV objects. In JSON, these should be children
//   of a root JSON object.
// - KV files just use pairs of tokens as keys and values. In JSON a colon
//   should be inserted between the key and the value.
// - KV entries (key-value pairs or objects) do not have commas between them,
//   which JSON requires.
// - KV objects can have multiple children with the same key, which is not
//   allowed in JSON. These should be converted to arrays.

class KeyValuesParser
{
public:
    KeyValuesParser(const QByteArray &input);

private:
    int nextNonWhitespaceCharacter(int from) const;
    void keyValuesToIntermediateJson(QByteArray &intJson);

    const QByteArray &m_Input;
};

#endif // KEYVALUESPARSER_H
