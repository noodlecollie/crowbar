#include "arbitrarydataa.h"

ArbitraryDataA::ArbitraryDataA(QObject *parent) : QObject(parent)
{
    m_VariantListProperty << QVariant(1) << QVariant(2) << QVariant(3);
    m_flFloatProperty = 2.2f;
}

ArbitraryDataA::~ArbitraryDataA()
{

}

int ArbitraryDataA::integerProperty() const
{
    return m_iIntegerProperty;
}

void ArbitraryDataA::setIntegerProperty(int value)
{
    if ( value == m_iIntegerProperty ) return;

    m_iIntegerProperty = value;
}


QString ArbitraryDataA::stringProperty() const
{
    return m_szStringProperty;
}

void ArbitraryDataA::setStringProperty(const QString &value)
{
    if ( value == m_szStringProperty ) return;

    m_szStringProperty = value;
}

QList<QVariant> ArbitraryDataA::variantListProperty() const
{
    return m_VariantListProperty;
}

float ArbitraryDataA::floatProperty() const
{
    return m_flFloatProperty;
}
