#pragma once

#include <QWidget>

class QLineEdit;
class QGroupBox;
class QTableWidget;
class QPushButton;

struct Params
{
    QVector<QVector<int>> x_node;
    QVector<QVector<int>> types_for_rod;
    QVector<QVector<int>> fixed_nodes;
    QVector<QVector<int>> centred_loads;
    QVector<QVector<int>> distr_loads;
    QVector<QVector<int>> types_descrip;
};

class Tables : public QWidget
{
    Q_OBJECT

public:
    Tables(QWidget *parent);
    ~Tables() = default;

signals:
    void passParams(const Params& params);

private slots:
    void setParamTables();
    void prepareParams();

private:
    QTableWidget * _t_1, * _t_2, * _t_3, * _t_4, * _t_5, * _t_6;
    QLineEdit* _line1, * _line2, * _line3, * _line4, * _line5;
    QGroupBox* _box1;
    QGroupBox* _box2;
    QPushButton* _calcBtn{};

    void clearVectors(Params& params);
    void createTopBox();
    void createTableWidgets();
    void addValuesToV(QTableWidget* table, QVector<QVector<int>>& vector);
    void setHeaders();
    void setWidgetSize();
};
