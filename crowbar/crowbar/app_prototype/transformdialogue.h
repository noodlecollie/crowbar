#ifndef TRANSFORMDIALOGUE_H
#define TRANSFORMDIALOGUE_H

#include <QDialog>
#include "mainwindow.h"
#include "mapdocument.h"
#include <QPointer>

namespace Ui {
class TransformDialogue;
}

class TransformDialogue : public QDialog
{
    Q_OBJECT
    
public:
    explicit TransformDialogue(MODEL_NAMESPACE::MapDocument* document, MainWindow *parent = 0);
    ~TransformDialogue();
    
public slots:
    void updateDocument();
    void populateFromDocument();
    
private:
    Ui::TransformDialogue *ui;
    QPointer<MODEL_NAMESPACE::MapDocument> m_pDocument;
};

#endif // TRANSFORMDIALOGUE_H
