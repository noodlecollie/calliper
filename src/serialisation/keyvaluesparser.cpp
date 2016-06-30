#include "keyvaluesparser.h"
#include <QStack>
#include "keyvaluestoken.h"
#include <QHash>
#include <QJsonArray>

KeyValuesParser::KeyValuesParser(const QByteArray &input) :
    m_Input(input)
{
}

int KeyValuesParser::nextNonWhitespaceCharacter(int from) const
{
    for ( int i = from; i < m_Input.length(); i++ )
    {
        if ( !KeyValuesToken::isWhitespace(m_Input.at(i)) )
            return i;
    }

    return -1;
}

void KeyValuesParser::keyValuesToIntermediateJson(QByteArray &intJson)
{
    // The depthTokens stack helps to keep track of what delimiters we should
    // be inserting into the output stream (eg. ':' between keys and values,
    // ',' between values and following keys, etc).
    // The rules are as follows:
    // - When a push is encountered, push a 0 onto the stack. This means
    //   that this level has had no tokens yet.
    // - When a token is encountered, increment the value on the top of the
    //   stack and use it to determine what to insert into the output.
    // - When a pop is encountered, pop the last value from the stack and
    //   increment the new top value. (A push followed by a pop counts as
    //   one whole value token at the given level.)
    // We also use the values on the stack to prefix each key with a unique
    // numerical identifier, since Qt doesn't support JSON objects which
    // have more than one identical key (but KV does).

    QStack<int> depthTokens;

    // Make sure we enclose everything within a root JSON object.
    intJson.append('{');
    depthTokens.push(0);

    int from = 0;
    int length = m_Input.length();

    while ( true )
    {
        // Find the beginning of the next token.
        from = nextNonWhitespaceCharacter(from);

        // If there's no next token, finish.
        if ( from >= length || from < 0 )
            break;

        // Get the next token.
        KeyValuesToken token(m_Input, from);

        // Do things depending on the type.
        if ( token == KeyValuesToken::TokenPush )
        {
            // Insert ':' if we've already had a key.
            if ( depthTokens.top() % 2 != 0 )
            {
                intJson.append(':');
            }
            else
            {
                // TODO: Error handling.
                // We've had a push before a corresponding key.
            }

            depthTokens.push(0);
        }
        else if ( token == KeyValuesToken::TokenPop )
        {
            if ( depthTokens.count() > 0 )
            {
                depthTokens.pop();
                if ( depthTokens.count() > 0 )
                {
                    depthTokens.top()++;
                }
                else
                {
                    // TODO: Error handling.
                    // We've had more pops than pushes.
                }
            }
            else
            {
                // TODO: Error handling.
                // We've had more pops than pushes.
            }
        }
        else if ( token.isString() )
        {
            if ( depthTokens.count() > 0 )
            {
                if ( depthTokens.top() != 0 )
                {
                    // Insert ':' or ',' depending on whether the number of
                    // tokens we've had is odd or even.
                    intJson.append(depthTokens.top() % 2 == 0 ? ',' : ':');
                }

                depthTokens.top()++;
            }
            else
            {
                // TODO: Error handling.
                // We've had more pops than pushes.
            }
        }

        // Actually write the token to the output.
        token.writeJson(m_Input, intJson, depthTokens.top() % 2 == 0
                        ? -1
                        : (depthTokens.top()-1)/2);

        // Advance the index past the token.
        from += token.length();

        // If we're past the end now, return.
        if ( from >= length )
            break;
    }

    // End the root object.
    intJson.append('}');
    depthTokens.pop();
}

void KeyValuesParser::convertNonUniqueKeysToArrays(QJsonObject &obj)
{
    typedef QHash<QString, QJsonArray> JsonTable;
    JsonTable itemTable;

    for ( QJsonObject::iterator it = obj.begin(); it != obj.end(); ++it )
    {
        QString key = it.key();
        int index = key.indexOf("_");
        if ( index >= 0 )
            key = key.mid(index+1);
        if ( key.isEmpty() )
            continue;

        if ( !itemTable.contains(key) )
        {
            QJsonArray array;
            array.append(it.value());
            itemTable.insert(key, array);
            continue;
        }

        itemTable[key].append(it.value());
    }

    obj = QJsonObject();
    for ( JsonTable::iterator it = itemTable.begin(); it != itemTable.end(); ++it )
    {
        if ( it->count() > 1 )
        {
            obj.insert(it.key(), *it);
        }
        else
        {
            obj.insert(it.key(), (*it)[0]);
        }
    }
}

void KeyValuesParser::convertNonUniqueKeysToArraysRecursive(QJsonObject &obj)
{
    for ( QJsonObject::iterator it = obj.begin(); it != obj.end(); ++it )
    {
        if ( (*it).isObject() )
        {
            QJsonObject child = (*it).toObject();
            convertNonUniqueKeysToArraysRecursive(child);
            *it = child;
        }
    }

    convertNonUniqueKeysToArrays(obj);
}
