#include "inputtranslator.h"
#include <QKeyEvent>
#include <QMetaObject>
#include <QGenericArgument>

#define COMMAND_NAME_PROPERTY "commandName"

MODEL_BEGIN_NAMESPACE

InputTranslator::InputTranslator(QObject *parent) :
    QObject(parent)
{
}

void InputTranslator::addKeyMapping(int key, QAction *action, bool forKeyDown)
{
    if ( forKeyDown ) m_KeyMapTable.insert(key, QPointer<QAction>(action));
    else m_KeyReleaseTable.insert(key, QPointer<QAction>(action));
}

void InputTranslator::removeKeyMapping(int key, bool forKeyDown)
{
    if ( forKeyDown ) m_KeyMapTable.remove(key);
    else m_KeyReleaseTable.remove(key);
}

QAction* InputTranslator::actionForKey(int key, bool forKeyDown) const
{
    if ( forKeyDown ) return m_KeyMapTable.value(key, QPointer<QAction>()).data();
    else return m_KeyReleaseTable.value(key, QPointer<QAction>()).data();
}

bool InputTranslator::containsKeyMapping(int key, bool forKeyDown) const
{
    if ( forKeyDown ) return m_KeyMapTable.contains(key);
    else return m_KeyReleaseTable.contains(key);
}

void InputTranslator::clearKeyMappings(bool forKeyDown)
{
    if ( forKeyDown ) m_KeyMapTable.clear();
    else m_KeyReleaseTable.clear();
}

void InputTranslator::clear()
{
    clearKeyMappings(true);
    clearKeyMappings(false);
    clearCommandMappings();
}

bool InputTranslator::eventFilter(QObject *obj, QEvent *e)
{
    // Only filter input events.
    if ( e->type() == QEvent::KeyPress || e->type() == QEvent::KeyRelease )
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(e);
        return (handleKey(keyEvent) | handleCommandKey(keyEvent));
    }

    return QObject::eventFilter(obj, e);
}

bool InputTranslator::handleKey(QKeyEvent *e)
{
    // Do we have a mapping for this key?
    QAction* a = actionForKey(e->key(), e->type() == QEvent::KeyPress);
    if ( !a ) return false;

    // Fire the action for this key.
    a->trigger();
    return true;
}

bool InputTranslator::handleCommandKey(QKeyEvent *e)
{
    if ( m_pInputTarget.isNull() ) return false;

    QString command = commandForKey(e->key());
    if ( command.isEmpty() ) return false;

    return QMetaObject::invokeMethod(m_pInputTarget.data(), command.toLatin1().constData(),
                                     Q_ARG(bool, e->type() == QEvent::KeyPress));
}

QObject* InputTranslator::inputTarget() const
{
    return m_pInputTarget.data();
}

void InputTranslator::setInputTarget(QObject *obj)
{
    if ( obj == m_pInputTarget.data() ) return;
    
    m_pInputTarget = obj;
    emit inputTargetChanged(m_pInputTarget.data());
}

void InputTranslator::addCommandMapping(int key, const QString &command)
{
    QString str = command.trimmed();
    if ( str.isEmpty() ) return;

    m_CommandTable.insert(key, str);
}

void InputTranslator::removeCommandMapping(int key)
{
    m_CommandTable.remove(key);
}

bool InputTranslator::containsCommandMapping(int key) const
{
    return m_CommandTable.contains(key);
}

void InputTranslator::clearCommandMappings()
{
    m_CommandTable.clear();
}

QString InputTranslator::commandForKey(int key) const
{
    return m_CommandTable.value(key, QString());
}

int InputTranslator::keyForCommand(const QString &command) const
{
    QString str = command.trimmed();
    if ( str.isEmpty() ) return -1;

    for ( CommandTable::const_iterator it = m_CommandTable.cbegin(); it != m_CommandTable.cend(); ++it )
    {
        QString s = it.value();
        if ( s == str ) return it.key();
    }

    return -1;
}

MODEL_END_NAMESPACE
