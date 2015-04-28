#ifndef TEXTURETABLEDOCKWIDGET_H
#define TEXTURETABLEDOCKWIDGET_H

#include <QDockWidget>
#include <QGraphicsScene>

namespace Ui {
class TextureTableDockWidget;
}

class TextureTableDockWidget : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit TextureTableDockWidget(QWidget *parent = 0);
    ~TextureTableDockWidget();
    
    QString currentTextureName() const;
    
signals:
    void textureChanged(const QString &textureName);
    
public slots:
    void updateTextureList(const QList<QString> &names);
    
private slots:
    void displayComboBoxItem(int index);
    
private:
    Ui::TextureTableDockWidget *ui;
    QGraphicsScene m_Scene;
};

#endif // TEXTURETABLEDOCKWIDGET_H
