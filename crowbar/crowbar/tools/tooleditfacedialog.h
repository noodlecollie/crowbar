#ifndef TOOLEDITFACEDIALOG_H
#define TOOLEDITFACEDIALOG_H

#include <QDialog>
#include "tools_global.h"
#include "facetextureinfo.h"

namespace Ui {
class TOOLSSHARED_EXPORT ToolEditFaceDialog;
}

TOOLS_BEGIN_NAMESPACE

class TOOLSSHARED_EXPORT ToolEditFaceDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ToolEditFaceDialog(QWidget *parent = 0);
    ~ToolEditFaceDialog();
    
    void getAttributesFrom(const MODEL_NAMESPACE::FaceTextureInfo* info);
    void setAttributesOn(MODEL_NAMESPACE::FaceTextureInfo* info);

signals:
    void attributesChanged();

private slots:
    void handleAttributesChanged();

private:
    Ui::ToolEditFaceDialog *ui;
    bool m_bDisableAttributeSignal;
};

TOOLS_END_NAMESPACE

#endif // TOOLEDITFACEDIALOG_H
