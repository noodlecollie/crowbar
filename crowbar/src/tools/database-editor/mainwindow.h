#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "map/database/database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void insertRowBefore();
    void insertRowAfter();
    void removeRow();
    void newSubkey();

private:
    Ui::MainWindow *ui;
    MODEL_NAMESPACE::Database* m_pDatabase;
};

#endif // MAINWINDOW_H