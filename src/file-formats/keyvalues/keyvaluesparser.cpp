#include "keyvaluesparser.h"
#include <QStack>
#include "keyvaluestoken.h"
#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include "exceptions/calliperexception.h"
#include <QtDebug>

namespace FileFormats
{
    namespace
    {
        // This is relatively expensive, as it scans the entire byte
        // array prefix for newline characters. It's intended only to
        // be called in the case of an exception, in order to provide
        // the line number of the given index.
        inline int numberOfNewlinesBeforeIndex(const QByteArray& arr, int index)
        {
            return arr.left(index).count('\n') + 1; // No newlines before means we're on line 1.
        }
    }

    class KeyValuesParser::InvalidSyntaxException : public CalliperUtil::CalliperException
    {
    public:
        void raise() const override { throw *this; }
        InvalidSyntaxException* clone() const override { return new InvalidSyntaxException(*this); }

        InvalidSyntaxException(int line, const QString& errorHint)
            : CalliperException(QString("Syntax error at line %1: %2")
                                .arg(line)
                                .arg(errorHint))
        {
        }

    private:
        QString m_strErrorHint;
    };

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

    void KeyValuesParser::keyValuesToIntermediateJson_x(QByteArray &intJson)
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
            if ( !token.isValid() )
            {
                throw InvalidSyntaxException(numberOfNewlinesBeforeIndex(m_Input, from),
                                             "Invalid syntax encountered.");
            }
            else if ( token == KeyValuesToken::TokenPush )
            {
                // Insert ':' if we've already had a key.
                if ( depthTokens.top() % 2 != 0 )
                {
                    intJson.append(':');
                }
                else
                {
                    // We've had a push before a corresponding key.
                    throw InvalidSyntaxException(numberOfNewlinesBeforeIndex(m_Input, from),
                                                 "'{' encountered before a key.");
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
                        // We've had more pops than pushes.
                        throw InvalidSyntaxException(numberOfNewlinesBeforeIndex(m_Input, from),
                                                     "'}' encountered without being matched with a beginning '{'.");
                    }
                }
                else
                {
                    // We've had more pops than pushes.
                    throw InvalidSyntaxException(numberOfNewlinesBeforeIndex(m_Input, from),
                                                 "'}' encountered without being matched with a beginning '{'.");
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
                    // We've had more pops than pushes.
                    throw InvalidSyntaxException(numberOfNewlinesBeforeIndex(m_Input, from),
                                                 "'}' encountered without being matched with a beginning '{'.");
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

    QJsonDocument KeyValuesParser::toJsonDocument(QString* errorString)
    {
        QByteArray intermediate;

        try
        {
            keyValuesToIntermediateJson_x(intermediate);
        }
        catch (CalliperUtil::CalliperException& exception)
        {
            if ( errorString )
                *errorString = QString("Error preprocessing KeyValues file: %1")
                    .arg(exception.errorHint());

            return QJsonDocument();
        }
        catch (...)
        {
            QString err("ERROR: Unknown exception thrown when parsing keyvalues file!");
            qCritical() << err;

            if ( errorString )
                *errorString = "Unknown exception thrown when parsing keyvalues file!";

            return QJsonDocument();
        }

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(intermediate, &error);

        if ( error.error != QJsonParseError::NoError )
        {
            if ( errorString )
                *errorString = QString("Error converting KeyValues to JSON: %1")
                    .arg(error.errorString());

            return QJsonDocument();
        }

        Q_ASSERT_X(doc.isObject(), Q_FUNC_INFO, "Expected JSON document to have a root object.");

        QJsonObject obj = doc.object();
        convertNonUniqueKeysToArraysRecursive(obj);
        doc.setObject(obj);
        return doc;
    }
}
