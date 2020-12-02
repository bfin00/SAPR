#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H
#include <QWidget>
struct Params;
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
private:
    QVector<double> _delta;
    QVector<QVector<double>> _A;
    QVector<double> _b;
    QVector<QVector<double>> _U;
    QVector<QVector<double>> _u;
    QVector<QVector<double>> _N;
    QVector<QVector<double>> _sigma;
    QVector<double> _startSigma;

    QLabel* lbl;

    void calcMatrixDelta(const Params& params);
    void calcMatrixA(const Params& params);
    void calcMatrixB(const Params& params);
    void calcMatrixU(const Params& params);
    void calcMatrixu_N(const Params& params);
    void calcMatrixes(const Params& params);

    void nullA(const Params& params);
    double getDeterminant(const QVector<QVector<double>> vect);
    int gauss(QVector<QVector<double>> a, QVector<double> & ans);

    void calcInPoint();
    void showTables();
    void showGraphics();
    void clearVectors();
    void checkSigmaCells(QTableWidget* table);
};

#endif // RESULTWINDOW_H
