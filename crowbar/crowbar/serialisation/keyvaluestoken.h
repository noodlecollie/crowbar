#ifndef KEYVALUESTOKEN_H
#define KEYVALUESTOKEN_H

#include <QByteArray>
#include <QPointer>
#include "serialisation_global.h"

SERIALISATION_BEGIN_NAMESPACE

class SERIALISATIONSHARED_EXPORT KeyValuesToken
{
public:
    enum TokenType
    {
        TokenInvalid,           // Not a valid token.
        TokenStringQuoted,      // Alphanumeric (plus underscore) token.
        TokenStringUnquoted,    // Token that was enclosed by quotes.
        TokenPush,              // Push (ie '{').
        TokenPop,               // Pop (ie '}').
        TokenComment            // Comment that began with '//'.
    };
    
    explicit KeyValuesToken(const QByteArray* array);
    explicit KeyValuesToken(const QByteArray *array, TokenType type, int begin, int length);
    
    TokenType type() const;
    void setType(TokenType type);
    
    const QByteArray* array() const;
    
    int begin() const;
    void setBegin(int val);
    
    int length() const;
    void setLength(int val);
    
    bool isValid() const;
    void invalidate();
    
    QByteArray arraySection() const;
    
    bool isString() const;
    bool isPush() const;
    bool isPop() const;
    
    // May not be within the bounds of the array.
    int nextReadPosition() const;
    void setNextReadPosition(int index);
    
private:
    const QByteArray*           m_pArray;
    TokenType                   m_iType;
    int                         m_iBegin;
    int                         m_iLength;
    int                         m_iNextReadPos;
};

SERIALISATION_END_NAMESPACE

#endif // KEYVALUESTOKEN_H
