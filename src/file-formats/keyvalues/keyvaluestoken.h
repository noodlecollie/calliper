#ifndef KEYVALUESTOKEN_H
#define KEYVALUESTOKEN_H

#include "file-formats_global.h"
#include <QByteArray>

namespace FileFormats
{
    class KeyValuesToken
    {
    public:
        enum TokenType
        {
            TokenInvalid = 0,
            TokenStringQuoted,      // "String"
            TokenStringUnquoted,    // String
            TokenPush,              // {
            TokenPop,               // }
            TokenLineComment,       // // I'm a line comment
            TokenBlockComment,      // /* I'm a block comment */
            TokenPreprocessor,      // # I'm a preprocessor
        };

        static QString tokenReadableName(TokenType type);

        KeyValuesToken();
        KeyValuesToken(const QByteArray &kv, int position);

        TokenType type() const;
        QString readableName() const;
        bool is(TokenType token) const;
        bool isValid() const;
        bool isString() const;
        bool shouldWriteJson() const;

        // Is an incomplete token: it's not invalid but the length is 0.
        bool isIncomplete() const;

        int length() const;
        int offset() const;

        void writeJson(const QByteArray &input, QByteArray &json, int prefix = -1);

        inline bool operator == (const TokenType &type) const
        {
            return is(type);
        }

        inline bool operator != (const TokenType &type) const
        {
            return !is(type);
        }

    private:
        void validateToken();
        static TokenType getToken(const QByteArray &arr, int position);
        static bool getTokenFromFirstChar(const QByteArray& arr, int position, TokenType &token);
        static bool getTokenFromFirstTwoChars(const char* str, TokenType &token);
        static int lengthOfToken(const QByteArray &arr, int position, int offset, const QByteArray &endDelim, bool eofIsDelimeter = false);
        static int lengthOfToken(const QByteArray &arr, int position, TokenType type);
        QByteArray getString(const QByteArray &input);

        TokenType   m_iTokenType;
        int         m_iBeginPos;
        int         m_iLength;
    };
}

#endif // KEYVALUESTOKEN_H
