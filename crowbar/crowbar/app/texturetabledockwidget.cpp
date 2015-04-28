#include "texturetabledockwidget.h"
#include "ui_texturetabledockwidget.h"
#include "applicationsettings.h"
#include <QGLTexture2D>
#include <QImage>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QtDebug>
#include <QTransform>

TextureTableDockWidget::TextureTableDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::TextureTableDockWidget)
{
    ui->setupUi(this);
    ui->imgTexture->setScene(&m_Scene);
}

TextureTableDockWidget::~TextureTableDockWidget()
{
    delete ui;
}

void TextureTableDockWidget::updateTextureList(const QList<QString> &names)
{
    ui->cbbTextureSelection->clear();
    
    foreach ( QString name, names )
    {
        int index = name.lastIndexOf("/");
        QString shortName = name.mid(index+1);
        ui->cbbTextureSelection->addItem(shortName, QVariant(name));
    }
    
    if ( ui->cbbTextureSelection->count() > 0 )
    {
        ui->cbbTextureSelection->setCurrentIndex(0);
    }
    else
    {
        ui->cbbTextureSelection->setCurrentIndex(-1);
    }
}

void TextureTableDockWidget::displayComboBoxItem(int index)
{
    if ( index < 0 || index >= ui->cbbTextureSelection->count()  )
    {
        m_Scene.clear();
        emit textureChanged(QString());
        return;
    }
    
    QString name = ui->cbbTextureSelection->itemData(index, Qt::UserRole).toString();
    QGLTexture2D* tex = appSettings().loadedTexture(name);
    Q_ASSERT(tex);
    QGraphicsPixmapItem* i = m_Scene.addPixmap(QPixmap::fromImage(tex->image()));
    float w = (float)i->pixmap().width();
    float h = (float)i->pixmap().height();
    Q_ASSERT(w != 0 && h != 0);
    
    float hscale = (float)ui->imgTexture->width()/w;
    float vscale = (float)ui->imgTexture->height()/h;
    QTransform scale(hscale, 0, 0,
                     0, vscale, 0,
                     0, 0, 1);
    i->setTransform(scale);
    
    ui->imgTexture->setSceneRect(m_Scene.itemsBoundingRect());
    ui->imgTexture->centerOn(i);
    emit textureChanged(name);
}

QString TextureTableDockWidget::currentTextureName() const
{
    if ( ui->cbbTextureSelection->currentIndex() < 0 ) return QString();
    return ui->cbbTextureSelection->currentData().toString();
}
