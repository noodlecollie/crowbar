#include "tooleditfacedialog.h"
#include "ui_tooleditfacedialog.h"

using namespace MODEL_NAMESPACE;

TOOLS_BEGIN_NAMESPACE

ToolEditFaceDialog::ToolEditFaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ToolEditFaceDialog)
{
    m_bDisableAttributeSignal = false;
    ui->setupUi(this);
    
    QList<QDoubleSpinBox*> dsblist = findChildren<QDoubleSpinBox*>();
    foreach ( QDoubleSpinBox* child, dsblist )
    {
        connect(child, SIGNAL(valueChanged(double)), this, SLOT(handleAttributesChanged()));
    }
    
    QList<QComboBox*> cbblist = findChildren<QComboBox*>();
    foreach ( QComboBox* child, cbblist )
    {
        connect(child, SIGNAL(currentIndexChanged(int)), this, SLOT(handleAttributesChanged()));
    }
    
    QList<QCheckBox*> cblist = findChildren<QCheckBox*>();
    foreach ( QCheckBox* child, cblist )
    {
        connect(child, SIGNAL(toggled(bool)), this, SLOT(handleAttributesChanged()));
    }
}

ToolEditFaceDialog::~ToolEditFaceDialog()
{
    delete ui;
}

void ToolEditFaceDialog::handleAttributesChanged()
{
    if ( !m_bDisableAttributeSignal ) emit attributesChanged();
}

void ToolEditFaceDialog::getAttributesFrom(const FaceTextureInfo *info)
{
    m_bDisableAttributeSignal = true;
    if ( !info )
    {
        ui->sbScaleX->setValue(0.25);
        ui->sbScaleY->setValue(0.25);
        ui->sbTrX->setValue(0);
        ui->sbTrY->setValue(0);
        ui->sbRotation->setValue(0);
        ui->sbLightmap->setValue(16);
        ui->sbUX->setValue(1);
        ui->sbUY->setValue(0);
        ui->sbUZ->setValue(0);
        ui->sbVX->setValue(0);
        ui->sbVY->setValue(1);
        ui->sbVZ->setValue(0);
        ui->cbAlignToFace->setChecked(false);
        ui->cbbParentSpace->setCurrentIndex(0);
        m_bDisableAttributeSignal = false;
        return;
    }
    
    ui->sbScaleX->setValue(info->scale().x());
    ui->sbScaleY->setValue(info->scale().y());

    ui->sbTrX->setValue(info->translation().x());
    ui->sbTrY->setValue(info->translation().y());

    ui->sbRotation->setValue(info->rotation());
    ui->sbLightmap->setValue(info->lightmapScale());

    ui->sbUX->setValue(info->textureUAxis().x());
    ui->sbUY->setValue(info->textureUAxis().y());
    ui->sbUZ->setValue(info->textureUAxis().z());

    ui->sbVX->setValue(info->textureVAxis().x());
    ui->sbVY->setValue(info->textureVAxis().y());
    ui->sbVZ->setValue(info->textureVAxis().z());

    ui->cbAlignToFace->setChecked(info->alignToFace());

    switch ( info->axisSpace() )
    {
        case FaceTextureInfo::ParentSpace:
        {
            ui->cbbParentSpace->setCurrentIndex(1);
            break;
        }

        case FaceTextureInfo::ObjectSpace:
        {
            ui->cbbParentSpace->setCurrentIndex(2);
            break;
        }

        default:
        {
            ui->cbbParentSpace->setCurrentIndex(0);
            break;
        }
    }

    m_bDisableAttributeSignal = false;
}

void ToolEditFaceDialog::setAttributesOn(FaceTextureInfo *info)
{
    Q_ASSERT(info);
    
    info->setScale(QVector2D(ui->sbScaleX->value(), ui->sbScaleY->value()));
    info->setTranslation(QVector2D(ui->sbTrX->value(), ui->sbTrY->value()));

    info->setRotation(ui->sbRotation->value());
    info->setLightmapScale(ui->sbLightmap->value());

    info->setTextureUAxis(QVector3D(ui->sbUX->value(), ui->sbUY->value(), ui->sbUZ->value()));
    info->setTextureVAxis(QVector3D(ui->sbVX->value(), ui->sbVY->value(), ui->sbVZ->value()));

    info->setAlignToFace(ui->cbAlignToFace->isChecked());

    switch ( ui->cbbParentSpace->currentIndex() )
    {
        case 1:
        {
            info->setAxisSpace(FaceTextureInfo::ParentSpace);
            break;
        }

        case 2:
        {
            info->setAxisSpace(FaceTextureInfo::ObjectSpace);
            break;
        }

        default:
        {
            info->setAxisSpace(FaceTextureInfo::WorldSpace);
            break;
        }
    }
}

TOOLS_END_NAMESPACE
