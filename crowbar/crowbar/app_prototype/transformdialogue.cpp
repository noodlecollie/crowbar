#include "transformdialogue.h"
#include "ui_transformdialogue.h"
#include "mapcomponent.h"
#include <QList>

using namespace MODEL_NAMESPACE;

TransformDialogue::TransformDialogue(MapDocument *document, MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::TransformDialogue)
{
    m_pDocument = document;
    ui->setupUi(this);
    populateFromDocument();
    
    if ( parent ) connect(parent, &MainWindow::selectedSetUpdated, this, &TransformDialogue::populateFromDocument);
}

TransformDialogue::~TransformDialogue()
{
    delete ui;
}

void TransformDialogue::populateFromDocument()
{
    if ( m_pDocument.isNull() ) return;
    
    QList<MapComponent*> list = m_pDocument->selectedSet().items();
    MapComponent* c = list.count() > 0 ? list.at(0) : NULL;
    MapWorld* w = m_pDocument->world();

    if ( c )
    {
        HierarchyTransform_New* tr = c->hierarchyTransform();
        HierarchyTransform_New* tr2 = w->hierarchyTransform();
        ui->xRot->setValue(tr->rotation().pitch());
        ui->yRot->setValue(tr->rotation().yaw());
        ui->zRot->setValue(tr->rotation().roll());
        ui->mapXRot->setValue(tr2->rotation().pitch());
        ui->mapYRot->setValue(tr2->rotation().yaw());
        ui->mapZRot->setValue(tr2->rotation().roll());
        ui->xTr->setValue(tr->translation().x());
        ui->yTr->setValue(tr->translation().y());
        ui->zTr->setValue(tr->translation().z());
        ui->mapXTr->setValue(tr2->translation().x());
        ui->mapYTr->setValue(tr2->translation().y());
        ui->mapZTr->setValue(tr2->translation().z());
        ui->xScl->setValue(tr->scale().x());
        ui->yScl->setValue(tr->scale().y());
        ui->zScl->setValue(tr->scale().z());
        ui->mapXScl->setValue(tr2->scale().x());
        ui->mapYScl->setValue(tr2->scale().y());
        ui->mapZScl->setValue(tr2->scale().z());
    }
    else
    {
        ui->xRot->setValue(0);
        ui->yRot->setValue(0);
        ui->zRot->setValue(0);
        ui->mapXRot->setValue(0);
        ui->mapYRot->setValue(0);
        ui->mapZRot->setValue(0);
        ui->xTr->setValue(0);
        ui->yTr->setValue(0);
        ui->zTr->setValue(0);
        ui->mapXTr->setValue(0);
        ui->mapYTr->setValue(0);
        ui->mapZTr->setValue(0);
        ui->xScl->setValue(0);
        ui->yScl->setValue(0);
        ui->zScl->setValue(0);
        ui->mapXScl->setValue(0);
        ui->mapYScl->setValue(0);
        ui->mapZScl->setValue(0);
    }
}

void TransformDialogue::updateDocument()
{
    if ( m_pDocument.isNull() ) return;

    EulerAngle angle((float)ui->xRot->value(),
                     (float)ui->yRot->value(),
                     (float)ui->zRot->value());

    QVector3D translation((float)ui->xTr->value(),
                          (float)ui->yTr->value(),
                          (float)ui->zTr->value());

    QVector3D scale((float)ui->xScl->value(),
                    (float)ui->yScl->value(),
                    (float)ui->zScl->value());
    
    MapComponentSet &set = m_pDocument->selectedSet();
    for ( MapComponentSet::Set::iterator it = set.begin(); it != set.end(); ++it )
    {
        HierarchyTransform_New* tr = (*it)->hierarchyTransform();
        tr->setRotation(angle);
        tr->setTranslation(translation);
        tr->setScale(scale);
    }
    
    EulerAngle mapAngle((float)ui->mapXRot->value(),
                        (float)ui->mapYRot->value(),
                        (float)ui->mapZRot->value());

    QVector3D maptr((float)ui->mapXTr->value(),
                    (float)ui->mapYTr->value(),
                    (float)ui->mapZTr->value());

    QVector3D mapscl((float)ui->mapXScl->value(),
                     (float)ui->mapYScl->value(),
                     (float)ui->mapZScl->value());

    HierarchyTransform_New* tr =  m_pDocument->world()->hierarchyTransform();
    tr->setRotation(mapAngle);
    tr->setTranslation(maptr);
    tr->setScale(mapscl);
    
    MainWindow* mw = qobject_cast<MainWindow*>(parent());
    if ( mw ) mw->updateViews();
}
