#include "keyvaluestoken.h"

SERIALISATION_BEGIN_NAMESPACE

KeyValuesToken::KeyValuesToken(const QByteArray *array) :
    m_pArray(array), m_iType(TokenInvalid), m_iBegin(-1), m_iLength(0), m_iNextReadPos(-1)
{
}

KeyValuesToken::KeyValuesToken(const QByteArray *array, TokenType type, int begin, int length) :
    m_pArray(array), m_iType(type), m_iBegin(-1), m_iLength(0), m_iNextReadPos(-1)
{
    setBegin(begin);
    setLength(length);
}

KeyValuesToken::TokenType KeyValuesToken::type() const
{
    return m_iType;
}

void KeyValuesToken::setType(TokenType type)
{
    m_iType = type;
}

const QByteArray* KeyValuesToken::array() const
{
    return m_pArray;
}

int KeyValuesToken::begin() const
{
    return m_iBegin;
}

void KeyValuesToken::setBegin(int val)
{
    if ( val < -1 ) val = -1;
     m_iBegin = val;
}

int KeyValuesToken::length() const
{
    return m_iLength;
}

void KeyValuesToken::setLength(int val)
{
    if ( val < 0 ) val = 0;
    m_iLength = val;
}

bool KeyValuesToken::isValid() const
{
    if ( !m_pArray ) return false;
    
    if ( m_iBegin < 0 || m_iBegin >= m_pArray->length() ) return false;
    if ( m_iLength < 1 || m_iLength > m_pArray->length() - m_iBegin ) return false;
    
    return true;
}

void KeyValuesToken::invalidate()
{
    setType(TokenInvalid);
    setBegin(-1);
    setLength(0);
}

QByteArray KeyValuesToken::arraySection() const
{
    if ( !isValid() ) return QByteArray();
    
    return array()->mid(begin(), length());
}

bool KeyValuesToken::isString() const
{
    return type() == TokenStringQuoted || type() == TokenStringUnquoted;
}

int KeyValuesToken::nextReadPosition() const
{
    return m_iNextReadPos;
}

void KeyValuesToken::setNextReadPosition(int index)
{
    if ( index < -1 ) index = -1;
    m_iNextReadPos = index;
}

bool KeyValuesToken::isPop() const
{
    return type() == TokenPop;
}

bool KeyValuesToken::isPush() const
{
    return type() == TokenPush;
}

SERIALISATION_END_NAMESPACE
