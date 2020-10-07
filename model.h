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
    QVector<QVector<int>> v_4;
    QVector<QVector<int>> v_5;
    QVector<QVector<int>> v_6;
private:
    void setUpWidgets();
};
#endif // MODEL_H
