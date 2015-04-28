#ifndef INPUTTRANSLATOR_H
#define INPUTTRANSLATOR_H

#include <QObject>
#include "model_global.h"
#include <QHash>
#include <QPointer>
#include <QAction>

// Translates key events from a filtered object to stored QActions.
// The translator does not own the actions stored.
// The mappings are one-to-one - if multiple actions are desired,
// fire these in response to the original action.

class QKeyEvent;

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT InputTranslator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* inputTarget READ inputTarget WRITE setInputTarget NOTIFY inputTargetChanged)
public:
    explicit InputTranslator(QObject *parent = 0);

    // Keys
    void addKeyMapping(int key, QAction* action, bool forKeyDown = true);
    QAction* actionForKey(int key, bool forKeyDown = true) const;
    void removeKeyMapping(int key, bool forKeyDown = true);
    bool containsKeyMapping(int key, bool forKeyDown = true) const;
    void clearKeyMappings(bool forKeyDown = true);

    // Mouse - TODO

    // Commands
    void addCommandMapping(int key, const QString &command);
    void removeCommandMapping(int key);
    bool containsCommandMapping(int key) const;
    void clearCommandMappings();
    QString commandForKey(int key) const;
    int keyForCommand(const QString &command) const;

    void clear();
    
    QObject* inputTarget() const;
    void setInputTarget(QObject* obj);

signals:
    void inputTargetChanged(QObject*);

public slots:

private slots:
    
    
protected:
    bool eventFilter(QObject *, QEvent *);

private:
    typedef QHash<int, QPointer<QAction> > InputTable;
    typedef QHash<int, QString> CommandTable;

    bool handleKey(QKeyEvent* e);
    bool handleCommandKey(QKeyEvent* e);

    InputTable      m_KeyMapTable;
    InputTable      m_KeyReleaseTable;
    CommandTable    m_CommandTable;
    
    QPointer<QObject> m_pInputTarget;
};

MODEL_END_NAMESPACE

#endif // INPUTTRANSLATOR_H
