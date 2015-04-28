#ifndef HIERARCHYEDITDIALOGUE_H
#define HIERARCHYEDITDIALOGUE_H

#include <QDialog>
#include "mapcomponentset.h"
#include "mapdocument.h"

namespace Ui {
class HierarchyEditDialogue;
}

class HierarchyEditDialogue : public QDialog
{
    Q_OBJECT
    
public:
    explicit HierarchyEditDialogue(QWidget *parent = 0);
    ~HierarchyEditDialogue();
    
    MODEL_NAMESPACE::MapDocument* document() const;
    void setDocument(MODEL_NAMESPACE::MapDocument* document);
    
    void readValues();
    void setValues();
    
signals:
    void documentChanged(MODEL_NAMESPACE::MapDocument*);
    
private slots:
    void translationChanged();
    void rotationChanged();
    void scaleChanged();
    
private:
    void readValuesFrom(const MODEL_NAMESPACE::MapComponentSet &selected);
    void setValuesOn(MODEL_NAMESPACE::MapComponentSet &selected);
    
    Ui::HierarchyEditDialogue *ui;
    bool m_bSingleTranslation;
    bool m_bSingleRotation;
    bool m_bSingleScale;
    bool m_bShouldUpdate;
    
    QPointer<MODEL_NAMESPACE::MapDocument> m_pDocument;
};

#endif // HIERARCHYEDITDIALOGUE_H
