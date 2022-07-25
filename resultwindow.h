#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H
#include <QWidget>
#include "Tables.h"
class QTableWidget;
class QLabel;
class ResultWindow: public QWidget
{
    Q_OBJECT
public:
    ResultWindow(QWidget *parent = nullptr);
    ~ResultWindow() = default;
signals:
    void returnBack();
public slots:
    void calcResuluts(const Params& params);

private slots:
    void returnToTables();
    void showAllValuesTables();
    void showTables();
    void showGraphics();
    void openPointWindow();
private:
    QVector<double> _delta;
    QVector<QVector<double>> _A;
    QVector<double> _b;
    QVector<QVector<double>> _U;
    QVector<QVector<double>> _u;
    QVector<QVector<double>> _N;
    QVector<QVector<double>> _sigma;
    QVector<double> _startSigma;

    QLineEdit* _calcLine;
    Params _params;

    void calcMatrixDelta(const Params& params);
    void calcMatrixA(const Params& params);
    void calcMatrixB(const Params& params);
    void calcMatrixU(const Params& params);
    void calcMatrixu_N(const Params& params);
    void calcMatrixes(const Params& params);

    void nullA(const Params& params);
    double getDeterminant(const QVector<QVector<double>> vect);
    int gauss(QVector<QVector<double>> a, QVector<double> & ans);
    int getRodNumber(const double& value, const double& allRodsL);

    void defineRod(QTableWidget* tabN, QTableWidget* tabU, QTableWidget* tabS);
    void clearVectors();
    void checkSigmaCells(QTableWidget* table);
    void setParams(const Params& params);
    void calcOneColumnTables(QTableWidget* tabN, QTableWidget* tabU, QTableWidget* tabS, const int& rodNumber, const double& x);
    void calcTwoColumnsTables(QTableWidget* tabN, QTableWidget* tabU, QTableWidget* tabS, const int& rodNumber, const double& x);
    void makeTables(QDialog* dialog);
    void makeTablesAllValues(QDialog* dialog);
    void fill_NUS_tables(QTableWidget* tabN, QTableWidget* tabU, QTableWidget* tabS);
    void fill_NUSAllValues_tables(QTableWidget* tableN);

    bool checkXValid();
};

#endif // RESULTWINDOW_H
