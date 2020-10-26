#pragma once

#include <QWidget>
#include <QTextStream>
class QLineEdit;
class QGroupBox;
class QTableWidget;
class QPushButton;

struct Params
{
    QVector<int> types_for_rod;
    QVector<int> fixed_nodes;
    QVector<int> centred_loads;
    QVector<int> distr_loads;
    QVector<QVector<double>> types_descrip;
};

class Tables : public QWidget
{
    Q_OBJECT

public:
    Tables(QWidget *parent);
    ~Tables() = default;


signals:
    void passParams(const Params& params);

public slots:
    bool save();
    void read();

private slots:
    void setParamTables();
    void prepareParams();

private:
    QTableWidget  * _t_2, * _t_3, * _t_4, * _t_5, * _t_6;
    QLineEdit* _line1, * _line2, * _line3;
    QGroupBox* _box1;
    QGroupBox* _box2;
    QPushButton* _calcBtn{};
    Params params;


    void clearVectors(Params& params);
    void createTopBox();
    void createTableWidgets();

    template <typename T>
    void addValuesToV(QTableWidget* table, QVector<QVector<T>>& vector);
    template <typename T>
    void addValuesToV(QTableWidget* table, QVector<T>& vector);

    void setHeaders();
    void setWidgetSize();
    bool checkLines();
    bool saveFile(const QString& fileName);
    bool checkFixedNodes();
    bool checkRods();
    void readFromFile(const QString& fileName);
    void readIntoVecs(QTextStream& in, QVector<int>& vec);
    void readIntoVecs(QTextStream& in, QVector<QVector<double>>& vec);
    void setTables();
    void setTable(const QVector<int>& vec, QTableWidget* table);
    void setTable(const QVector<QVector<double>>& vec, QTableWidget* table);

    template <typename T>
    void writeToFile(QTextStream& out, const QVector<QVector<T>>& vec);
    template <typename T>
    void writeToFile(QTextStream& out, const QVector<T>& vec);

    void drawConstruction();
};
