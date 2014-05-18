#include "keyvaluesnode.h"

DATASTRUCTURES_BEGIN_NAMESPACE

KeyValuesNode::KeyValuesNode(KeyValuesNode *parent) : TreeNode(parent),
    m_szKey(), m_Value()
{
}

KeyValuesNode::~KeyValuesNode()
{
}

QString KeyValuesNode::key() const
{
    return m_szKey;
}

void KeyValuesNode::setKey(const QString &key)
{
    m_szKey = key;
}

void KeyValuesNode::setKey(const char *str)
{
    setKey(QString(str));
}

QVariant KeyValuesNode::value() const
{
    return m_Value;
}

const QVariant& KeyValuesNode::constValue() const
{
    return m_Value;
}

void KeyValuesNode::setValue(bool value)
{
    m_Value.setValue(value);
}

void KeyValuesNode::setValue(const QString &value)
{
    m_Value.setValue(value);
}

void KeyValuesNode::setValue(const QVariant &value)
{
    m_Value = value;
}

void KeyValuesNode::setValue(float value)
{
    m_Value.setValue(value);
}

void KeyValuesNode::setValue(int value)
{
    m_Value.setValue(value);
}

void KeyValuesNode::setValue(const QVector2D &value)
{
    m_Value.setValue(value);
}

void KeyValuesNode::setValue(const QVector3D &value)
{
    m_Value.setValue(value);
}

DATASTRUCTURES_END_NAMESPACE
