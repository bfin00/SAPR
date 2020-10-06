#ifndef MODEL_H
#define MODEL_H
#include <QWidget>
#include <QVector>
#include <QTableWidget>
class Model: public QWidget
{
public:
    Model(QWidget *parent = nullptr);
    QVector<int> v_1;
    QVector<int> v_2;
    QVector<int> v_3;
    QVector<QVector<int>> vv_4;
    QVector<QVector<int>> vv_2;
    QVector<QVector<int>> vvv_2;
private:
    void setUpWidgets();
public slots:
    void add_row(int c, int r, QTableWidget* t);
};
#endif // MODEL_H
