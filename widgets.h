#ifndef WIDGETS_H
#define WIDGETS_H
#include "Model.h"
#include <QPoint>
#include <QPixmap>
#include <QVector>
#include <QWidget>
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
class QGroupBox;
class QLineEdit;
class Widgets : public QWidget
{
public:
    explicit Widgets(QWidget *parent = nullptr);
    Model *model;
private:
    void setupWidgets();
    QGroupBox *horizontalGroupBox;
    QLineEdit *lineEdit;
    QGroupBox *box1;
    QGroupBox *box2;
    QTableWidget *t_top, *t_1, *t_2, *t_3, *t_4, *t_5, *t_6;
    QLineEdit *line1, *line2, *line3, *line4, *line5;

    void setTab(int s);
private slots:
    void setUpTables();
    void addValuesToV(int c, int r);
};

#endif // WIDGETS_H
