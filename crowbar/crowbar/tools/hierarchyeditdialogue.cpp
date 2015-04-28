#include "hierarchyeditdialogue.h"
#include "ui_hierarchyeditdialogue.h"
#include "eulerangle.h"
#include "mapcomponent.h"

using namespace MODEL_NAMESPACE;

HierarchyEditDialogue::HierarchyEditDialogue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HierarchyEditDialogue)
{
    ui->setupUi(this);
    m_bShouldUpdate = true;
    
    connect(ui->sbTransX, SIGNAL(valueChanged(double)), this, SLOT(translationChanged()));
    connect(ui->sbTransY, SIGNAL(valueChanged(double)), this, SLOT(translationChanged()));
    connect(ui->sbTransZ, SIGNAL(valueChanged(double)), this, SLOT(translationChanged()));
    
    connect(ui->sbRotPitch, SIGNAL(valueChanged(double)), this, SLOT(rotationChanged()));
    connect(ui->sbRotYaw, SIGNAL(valueChanged(double)), this, SLOT(rotationChanged()));
    connect(ui->sbRotRoll, SIGNAL(valueChanged(double)), this, SLOT(rotationChanged()));
    
    connect(ui->sbScaleX, SIGNAL(valueChanged(double)), this, SLOT(scaleChanged()));
    connect(ui->sbScaleY, SIGNAL(valueChanged(double)), this, SLOT(scaleChanged()));
    connect(ui->sbScaleZ, SIGNAL(valueChanged(double)), this, SLOT(scaleChanged()));
}

HierarchyEditDialogue::~HierarchyEditDialogue()
{
    delete ui;
}

void HierarchyEditDialogue::readValuesFrom(const Model::MapComponentSet &selected)
{
    if ( !m_bShouldUpdate || selected.count() < 1 ) return;
    
    m_bSingleTranslation = true;
    m_bSingleRotation = true;
    m_bSingleScale = true;
    QVector3D translation;
    EulerAngle rotation;
    QVector3D scale;
    
    bool first = true;
    for ( MapComponentSet::Set::const_iterator it = selected.cbegin();
          it != selected.cend();
          ++it )
    {
        const MapComponent* c = *it;
        
        if ( first )
        {
            translation = c->hierarchyTransform()->translation();
            rotation = c->hierarchyTransform()->rotation();
            scale = c->hierarchyTransform()->scale();
            first = false;
            continue;
        }
        
        QVector3D t = c->hierarchyTransform()->translation();
        if ( t != translation )
        {
            m_bSingleTranslation = false;
        }
        
        EulerAngle r = c->hierarchyTransform()->rotation();
        if ( r != rotation )
        {
            m_bSingleRotation = false;
        }
        
        QVector3D s = c->hierarchyTransform()->scale();
        if ( s != scale )
        {
            m_bSingleScale = false;
        }
    }
    
    QList<QDoubleSpinBox*> list = findChildren<QDoubleSpinBox*>();
    foreach ( QObject* o, list )
    {
        o->blockSignals(true);
    }
    
    ui->sbTransX->setValue(translation.x());
    ui->sbTransY->setValue(translation.y());
    ui->sbTransZ->setValue(translation.z());
    
    ui->sbRotPitch->setValue(rotation.pitch());
    ui->sbRotYaw->setValue(rotation.yaw());
    ui->sbRotRoll->setValue(rotation.roll());
    
    ui->sbScaleX->setValue(scale.x());
    ui->sbScaleY->setValue(scale.y());
    ui->sbScaleZ->setValue(scale.z());
    
    foreach ( QObject* o, list )
    {
        o->blockSignals(false);
    }
}

void HierarchyEditDialogue::setValuesOn(MapComponentSet &selected)
{
    if ( selected.count() < 1 ) return;
    
    m_bShouldUpdate = false;
    
    QVector3D translation(ui->sbTransX->value(),
                          ui->sbTransY->value(),
                          ui->sbTransZ->value());
    EulerAngle rotation(ui->sbRotPitch->value(),
                        ui->sbRotYaw->value(),
                        ui->sbRotRoll->value());
    QVector3D scale(ui->sbScaleX->value(),
                    ui->sbScaleY->value(),
                    ui->sbScaleZ->value());
    
    for ( MapComponentSet::Set::iterator it = selected.begin();
          it != selected.end();
          ++it )
    {
        MapComponent* c = *it;
        if ( m_bSingleTranslation )
        {
            c->hierarchyTransform()->setTranslation(translation);
        }
        if ( m_bSingleRotation )
        {
            c->hierarchyTransform()->setRotation(rotation);
        }
        if ( m_bSingleScale )
        {
            c->hierarchyTransform()->setScale(scale);
        }
    }
    
    m_bShouldUpdate = true;
}

void HierarchyEditDialogue::translationChanged()
{
    m_bSingleTranslation = true;
    setValues();
}

void HierarchyEditDialogue::rotationChanged()
{
    m_bSingleRotation = true;
    setValues();
}

void HierarchyEditDialogue::scaleChanged()
{
    m_bSingleScale = true;
    setValues();
}

MapDocument* HierarchyEditDialogue::document() const
{
    return m_pDocument.data();
}

void HierarchyEditDialogue::setDocument(MapDocument *document)
{
    if ( m_pDocument.data() == document ) return;
    
    m_pDocument = document;
    emit documentChanged(m_pDocument.data());
}

void HierarchyEditDialogue::readValues()
{
    if ( m_pDocument.isNull() ) return;
    
    readValuesFrom(document()->selectedSet());
}

void HierarchyEditDialogue::setValues()
{
    if ( m_pDocument.isNull() ) return;
    
    setValuesOn(document()->selectedSet());
}
