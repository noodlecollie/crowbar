/** \file keyvaluesnode.h
 * \brief Defines the KeyValuesNode class.
 */

#ifndef KEYVALUESNODE_H
#define KEYVALUESNODE_H

#include "datastructures_global.h"
#include "treenode.h"

#include <QHash>
#include <QString>
#include <QVariant>
#include <QObject>
#include <QVector2D>
#include <QVector3D>

DATASTRUCTURES_BEGIN_NAMESPACE

/**
 * @brief Implements a tree node for keyvalues trees. The key is held as a string and the value as a QVariant.
 * Child nodes can be added to a keyvalues node to form a tree.
 * Note that the node allows storage of a value even if it is not a leaf node, which Valve's keyvalues files
 * don't allow - it is recommended to only store values in leaf nodes.
 */
class DATASTRUCTURESSHARED_EXPORT KeyValuesNode : public QObject, public TreeNode
{
    Q_OBJECT
    Q_PROPERTY(QString key READ key WRITE setKey)
    Q_PROPERTY(QVariant value READ value WRITE setValue)
public:
     /**
      * @brief Constructor
      * @param parent Parent node. Note that this does not explicitly add this node as a child inside the parent node.
      */
    KeyValuesNode(KeyValuesNode* parent = NULL);

    /**
     * @brief Destructor.
     */
    virtual ~KeyValuesNode();

    /**
     * @brief Returns this node's key.
     * @return Key as a string.
     */
    QString key() const;
    
    /**
     * @brief Sets this node's key.
     * @param key String to set.
     */
    void setKey(const QString &key);
    
    /**
     * @brief Set this node's key.
     * @param str String to set.
     */
    void setKey(const char* str);
    
    /**
     * @brief Returns this node's value, or a default-constructed QVariant if this node is not a leaf.
     * @return Value of the node as a QVariant.
     */
    QVariant value() const;
    
    /**
     * @brief Returns a const reference to this node's QVariant value.
     * @return 
     */
    const QVariant& constValue() const;
    
    /**
     * @brief Sets the node's value.
     * @param value Value as QVariant.
     */
    void setValue(const QVariant &value);
    
    /**
     * @brief Sets the node's value.
     * @param value Value as QString.
     */
    void setValue(const QString &value);
    
    /**
     * @brief Sets the node's value.
     * @param value Value as float.
     */
    void setValue(float value);
    
    /**
     * @brief Sets the node's value.
     * @param value Value as bool.
     */
    void setValue(bool value);
    
    /**
     * @brief Sets the node's value.
     * @param value Value as int.
     */
    void setValue(int value);
    
    /**
     * @brief Sets the node's value.
     * @param value Value as QVector3D.
     */
    void setValue(const QVector3D &value);
    
    /**
     * @brief Sets the node's value.
     * @param value Value as QVector2D.
     */
    void setValue(const QVector2D &value);
    
    // Convenience overrides.
    /**
     * @brief Adds a child to this node.
     * @param node Node to add.
     */
    virtual void addChild(KeyValuesNode* node)         { TreeNode::addChild(node); }
    
    /**
     * @brief Removes a child from this node at the given index.
     * @param index Index to remove from.
     * @return Object removed, or NULL if the index was invalid.
     */
    virtual KeyValuesNode* removeChild(int index)      { return dynamic_cast<KeyValuesNode*>(TreeNode::removeChild(index)); }
    
    /**
     * @brief Returns the child at a given index.
     * @param index Index to use.
     * @return Child at this index, or NULL if the index was invalid.
     */
    virtual KeyValuesNode* childAt(int index) const    { return dynamic_cast<KeyValuesNode*>(TreeNode::childAt(index)); }
    
    /**
     * @brief Returns the parent of this node.
     * @note If the parent hasn't been informed of this node, it may not be one of the parent's listed children.
     * @return Parent recorded in this node, or NULL if this node is the root of a tree.
     */
    virtual KeyValuesNode* parent() const              { return dynamic_cast<KeyValuesNode*>(TreeNode::parent()); }
    
    /**
     * @brief Sets the parent of this node.
     * @note Does not add this node as a child of the parent.
     * @param parent Parent to set.
     */
    virtual void setParent(KeyValuesNode* parent)      { TreeNode::setParent(parent); }
    
    /**
     * @brief Creates a new child for this node and returns it.
     * @return Child node that was created.
     */
    virtual inline KeyValuesNode* addChild()
    {
        KeyValuesNode* node = new KeyValuesNode();
        addChild(node);
        return node;
    }

private:
    QString     m_szKey;    /**< Key for this node. */
    QVariant    m_Value;    /**< Value for this node. */
};

DATASTRUCTURES_END_NAMESPACE

#endif // KEYVALUESNODE_H
