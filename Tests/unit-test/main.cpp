#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QThread>
#include <QPushButton>
#include <QObject>
#include "handler.h"
#include "customspin.h"

class CustomWidget : public QWidget
{
public:
    explicit CustomWidget(QWidget *parent = 0) : QWidget(parent)
    {
        layout = new QVBoxLayout();
        this->setLayout(layout);
        
        spin = new CustomSpin();
        layout->addWidget(spin);
        
        button = new QPushButton();
        button->setText("Push");
        layout->addWidget(button);
        
        h = new Handler();
        connect(button, SIGNAL(clicked()), h, SLOT(in()));
        connect(h, SIGNAL(out()), spin, SLOT(increment()));
        
        thread = new QThread();
        h->moveToThread(thread);
        thread->start();
        
    }
    
    virtual ~CustomWidget()
    {
    }
    
private:
    QVBoxLayout* layout;
    CustomSpin* spin;
    QPushButton* button;
    QThread* thread;
    Handler* h;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CustomWidget c;
    c.resize(800, 600);
    c.show();
    
    return app.exec();
}
