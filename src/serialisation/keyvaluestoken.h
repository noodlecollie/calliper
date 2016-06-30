#ifndef KEYVALUESTOKEN_H
#define KEYVALUESTOKEN_H

#include <QByteArray>

class KeyValuesToken
{
public:
    enum TokenType
    {
        TokenInvalid = 0,
        TokenStringQuoted,
        TokenStringUnquoted,
        TokenPush,
        TokenPop,
        TokenLineComment,
        TokenBlockComment,
        TokenPreprocessor,
    };

    KeyValuesToken();
    KeyValuesToken(const QByteArray &kv, int position);

    TokenType type() const;
    bool is(TokenType token) const;
    bool isValid() const;

    static bool isWhitespace(char ch);
    static bool isAlphaNumeric(char ch);

private:
    static TokenType getToken(const QByteArray &arr, int position);
    static bool getTokenFromFirstChar(char ch, TokenType &token);
    static bool getTokenFromFirstTwoChars(const char* str, TokenType &token);
    static int lengthOfToken(const QByteArray &arr, int begin, const QByteArray &endDelim);
    static int lengthOfToken(const QByteArray &arr, int position, TokenType type);

    TokenType   m_iTokenType;
    int         m_iBeginPos;
    int         m_iLength;
};

#endif // KEYVALUESTOKEN_H
