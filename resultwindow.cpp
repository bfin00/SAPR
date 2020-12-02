#include "Tables.h"
#include "resultwindow.h"
#include <QtWidgets>
#include <exception>
#define EPS 0.000001
#define INF 1000000
ResultWindow::ResultWindow(QWidget* parent): QWidget(parent) {};
void ResultWindow::calcResuluts(const Params& params)
{
    clearVectors();
    calcMatrixes(params);
    auto mainLayout = new QVBoxLayout(this);
    auto topBox = new QGroupBox;
    auto botBox = new QGroupBox;
    auto topLayout = new QVBoxLayout(topBox);
    auto calcBtn = new QPushButton(tr("Рассчитать"));
    lbl = new QLabel("Заданная точка X");
    auto calcLine = new QLineEdit;
    auto returnBut = new QPushButton(tr("Назад"));
    auto tablesBut = new QPushButton(tr("Таблицы"));
    auto graphicsBut = new QPushButton(tr("Эпюры"));
    returnBut->setFixedSize({100, 50});
    auto botLayout = new QHBoxLayout(botBox);

    botLayout->addWidget(lbl);
    botLayout->addWidget(calcLine);
    botLayout->addWidget(calcBtn);

    topLayout->addWidget(returnBut);
    topLayout->addWidget(tablesBut);
    topLayout->addWidget(graphicsBut);
    mainLayout->addWidget(topBox);
    mainLayout->addWidget(botBox);
    connect(returnBut, &QPushButton::clicked, this, &ResultWindow::returnToTables);
    connect(tablesBut, &QPushButton::clicked, this, &ResultWindow::showTables);
    connect(graphicsBut, &QPushButton::clicked, this, &ResultWindow::showGraphics);
    connect(calcBtn, &QPushButton::clicked, this, &ResultWindow::calcInPoint);
}

void ResultWindow::calcMatrixDelta(const Params &params)
{
    _delta.resize(params.centred_loads.size());
    for (auto i = 0; i < _delta.size(); ++i)
        _delta[i] = 0;
    QVector<QVector<double>> matrixForGauss = _A;
    for (auto i = 0; i < matrixForGauss.size(); ++i)
        matrixForGauss[i].push_back(_b[i]);

    gauss(matrixForGauss, _delta);
    if (params.fixed_nodes[0] != 0)
        _delta[0] = 0;
    if (params.fixed_nodes[1] != 0)
        _delta[_delta.size()-1] = 0;
}
void ResultWindow::calcMatrixB(const Params &params)
{
    _b.resize(params.centred_loads.size());
    for (auto i = 0; i < _b.size(); ++i)
    {
        if(i == 0)
            _b[i] = params.centred_loads[i] + (params.distr_loads[i] * params.type_for_each_rod[i][4])/2;
        else if (i == _b.size() - 1)
            _b[i] = params.centred_loads[i] + (params.distr_loads[i-1] * params.type_for_each_rod[i-1][4])/2;
        else
            _b[i] = params.centred_loads[i] + (params.distr_loads[i] * params.type_for_each_rod[i-1][4])/2
                    + (params.distr_loads[i-1] * params.type_for_each_rod[i-1][4])/2;
    }
    if (params.fixed_nodes[0] != 0)
        _b[0] = 0;
    if (params.fixed_nodes[1] != 0)
        _b[_b.size()-1] = 0;
}
void ResultWindow::calcMatrixA(const Params &params)
{
    _A.resize(params.centred_loads.size());
    for (auto i = 0; i < params.centred_loads.size(); ++i)
        _A[i].resize(params.centred_loads.size());
    for (auto i = 0; i < _A.size(); ++i)
    {
        for (auto j = 0; j < _A[i].size(); ++j)
        {
            if (i ==_A.size() - 1 && j == _A[i].size() - 1)
            {
                double A = params.type_for_each_rod[i-1][1];
                double E = params.type_for_each_rod[i-1][2];
                double L = params.type_for_each_rod[i-1][4];
                _A[i][j] = (E * A)/L;
            }
            else if (i == j && i !=_A.size() - 1 && j != _A[i].size() - 1)
            {
                double A =params.type_for_each_rod[i][1];
                double E = params.type_for_each_rod[i][2];
                double L = params.type_for_each_rod[i][4];
                if (i == 0)
                {
                    _A[i][j] = (E * A)/L;
                    _A[i+1][j] = -(E * A)/L;
                    _A[i][j+1] = -(E * A)/L;
                }
                else
                {
                    double previous = (params.type_for_each_rod[i-1][1] *params.type_for_each_rod[i-1][2])/params.type_for_each_rod[i-1][4];
                    _A[i][j] = previous + (E * A)/L;
                    _A[i+1][j] = -(E * A)/L;
                    _A[i][j+1] = -(E * A)/L;
                }
            }
        }
    }
    nullA(params);
}

void ResultWindow::returnToTables() {emit returnBack();}

int ResultWindow::gauss (QVector<QVector<double>> a, QVector<double> & ans)
{
    int n = a.size();
    int m = a[0].size() - 1;

    QVector<int> where (m, -1);
    for (int col=0, row=0; col<m && row<n; ++col)
    {
        int sel = row;
        for (int i=row; i<n; ++i)
            if (abs (a[i][col]) > abs (a[sel][col]))
                sel = i;
        if (abs (a[sel][col]) < EPS)
            continue;
        for (int i=col; i<=m; ++i)
        {
            //swap(a[sel][i], a[row][i]);
            double temp = a[sel][i];
            a[sel][i] = a[row][i];
            a[row][i] = temp;
        }
        where[col] = row;

        for (int i=0; i<n; ++i)
            if (i != row)
            {
                double c = a[i][col] / a[row][col];
                for (int j=col; j<=m; ++j)
                    a[i][j] -= a[row][j] * c;
            }
        ++row;
    }

    ans.resize(m);
    for (int i=0; i<m; ++i)
        if (where[i] != -1)
            ans[i] = a[where[i]][m] / a[where[i]][i];
    for (int i=0; i<n; ++i)
    {
        double sum = 0;
        for (int j=0; j<m; ++j)
            sum += ans[j] * a[i][j];
        if (abs (sum - a[i][m]) > EPS)
            return 0;
    }

    for (int i=0; i<m; ++i)
        if (where[i] == -1)
            return INF;
    return 1;
}
void ResultWindow::nullA(const Params &params)
{
    if (params.fixed_nodes[0] != 0)
    {
        for (auto i = 0; i < _A.size(); ++i)
        {
            if (i != 0)
            {
                _A[0][i] = 0;
                _A[i][0] = 0;
            }
            else
                _A[0][i] = 1;
        }
    }
    if (params.fixed_nodes[1] != 0)
    {
        for (auto i = _A.size()-1; i >= 0; --i)
        {
            if (i != _A.size()-1)
            {
                _A[_A.size()-1][i] = 0;
                _A[i][_A.size()-1] = 0;
            }
            else
                _A[_A.size()-1][i] = 1;
        }
    }
}
void ResultWindow::calcMatrixU(const Params& params)
{
    _U.resize(params.type_for_each_rod.size());

    for (auto i = 0; i < _U.size(); ++i)
    {
        _U[i].resize(2);
        _U[i] = {_delta[i], _delta[i+1]};
    }
}
void ResultWindow::calcMatrixu_N(const Params &params)
{
    double x = 0.2;
    _u.resize(params.type_for_each_rod.size());
    _N.resize(_u.size());

    for (auto i = 0; i < _u.size(); ++i)
        _u[i].resize(static_cast<int>(params.type_for_each_rod[i][4]/x));
    for (auto i = 0; i < _u.size(); ++i)
    {
        _N[i].resize(_u[i].size());
        x = 0;
        double E = params.type_for_each_rod[i][2];
        double A = params.type_for_each_rod[i][1];
        double L = params.type_for_each_rod[i][4];
        double q = params.distr_loads[i];
        for (auto j = 0; j < _u[i].size(); ++j)
        {
            _u[i][j] = (1 - x/L) * _U[i][0] + (x / L) * _U[i][1] + (q * L*L * x)/(2*E*A*L) * (1 - x/L);
            _N[i][j] = ((E * A) / L) * (_U[i][1] - _U[i][0]) + ((q * L) / 2) * (1 - 2 * (x / L));
            x += 0.2;
        }
        _u[i].push_back((1 - x/L) * _U[i][0] + (x / L) * _U[i][1] + (q * L*L * x)/(2*E*A*L) * (1 - x/L));
        _N[i].push_back(((E * A) / L) * (_U[i][1] - _U[i][0]) + ((q * L) / 2) * (1 - 2 * (x / L)));
        QVector<double> sigmaValues = {_N[i][0], _N[i][_N[i].size()-1]};
        _sigma.push_back(sigmaValues);
        _startSigma.push_back(params.type_for_each_rod[i][3]);
    }
}

void ResultWindow::calcMatrixes(const Params& params)
{
    calcMatrixA(params);
    calcMatrixB(params);
    calcMatrixDelta(params);
    calcMatrixU(params);
    calcMatrixu_N(params);
}

void ResultWindow::showTables()
{
    auto tablesWindow = new QDialog(this, Qt::WindowStaysOnTopHint);
    tablesWindow->setMinimumSize(1000, 600);
    auto layout = new QHBoxLayout(tablesWindow);
    auto tableN = new QTableWidget(_N.size(), 2, tablesWindow);
    auto tableU = new QTableWidget(_u.size(), 2, tablesWindow);
    auto tableSigma = new QTableWidget(_u.size(), 2, tablesWindow);
    tableN->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Nx(0)")));
    tableN->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Nx(L)")));
    tableU->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Ux(0)")));
    tableU->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Ux(L)")));
    tableSigma->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("σ(0)")));
    tableSigma->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("σ(L)")));
    layout->addWidget(tableN);
    layout->addWidget(tableU);
    layout->addWidget(tableSigma);
    for (auto i = 0; i < tableN->rowCount(); ++i)
    {
        _N[i][0] = round(_N[i][0]*1000)/1000;
        _N[i][_N[i].size()-1] = round(_N[i][_N[i].size()-1] * 1000) / 1000;
        QString item1 = QString::number(_N[i][0]);
        QString item2 = QString::number(_N[i][_N[i].size()-1]);
        tableN->setItem(i, 0, new QTableWidgetItem(item1));
        tableN->setItem(i, 1, new QTableWidgetItem(item2));
    }
    for (auto i = 0; i < tableU->rowCount(); ++i)
    {
        _u[i][0] = round(_u[i][0]*1000)/1000;
        _u[i][_u[i].size()-1] = round(_u[i][_u[i].size()-1] * 1000) / 1000;
        QString item1 = QString::number(_u[i][0]);
        QString item2 = QString::number(_u[i][_u[i].size()-1]);
        tableU->setItem(i, 0, new QTableWidgetItem(item1));
        tableU->setItem(i, 1, new QTableWidgetItem(item2));
    }
    for (auto i = 0; i < tableSigma->rowCount(); ++i)
    {
        QString item1 = QString::number(_sigma[i][0]);
        QString item2 = QString::number(_sigma[i][1]);
        tableSigma->setItem(i, 0, new QTableWidgetItem(item1));
        tableSigma->setItem(i, 1, new QTableWidgetItem(item2));
    }
    checkSigmaCells(tableSigma);
    tablesWindow->show();
}

void ResultWindow::showGraphics()
{

}

void ResultWindow::clearVectors()
{
    _startSigma.clear();
    _delta.clear();
    _A.clear();
    _U.clear();
    _u.clear();
    _N.clear();
    _sigma.clear();
    _b.clear();
}

void ResultWindow::checkSigmaCells(QTableWidget* table)
{
    for (auto i = 0; i < table->rowCount(); ++i)
        for (auto j = 0; j < table->columnCount(); ++j)
            if (abs(_sigma[i][j]) > abs(_startSigma[i]))
                table->item(i,j)->setBackground(Qt::red);
}

void ResultWindow::calcInPoint()
{
    auto calcPointWindow = new QDialog(this, Qt::WindowStaysOnTopHint);
    calcPointWindow->setMinimumSize(1000, 600);
    auto layout = new QHBoxLayout(calcPointWindow);
    auto lblN = new QLabel(tr("N(x)"));
    auto lblU = new QLabel(tr("U(x)"));
    auto lblS = new QLabel(tr("Nx"));
}
