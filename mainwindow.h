#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Model.h"
#include "widgets.h"
#include <QMainWindow>
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    void SetupMenus();
    void SetupWidgets();
    Widgets *w1;
    Model *m1;
};
#endif // MAINWINDOW_H
