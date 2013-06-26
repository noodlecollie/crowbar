#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QWidget>

class QTextEdit;
class QBoxLayout;
class QCloseEvent;

class LogWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LogWindow(QWidget *parent = 0);

    qreal getFontSize();
    
signals:
    
public slots:
    void show();
    void showAndRaise();

private:
    QTextEdit*  m_pText;    // Main text window.
    QBoxLayout* m_pLayout;
};

#endif // LOGWINDOW_H
