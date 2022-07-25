#include "resultwindow.h"
#include <QtWidgets>
#include <exception>
#include <QChartView>
#include <QLineSeries>
#include <QPointF>

#define EPS 0.000001
#define INF 1000000
QT_CHARTS_USE_NAMESPACE
ResultWindow::ResultWindow(QWidget* parent): QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    auto topBox = new QGroupBox;
    auto botBox = new QGroupBox;
    auto topLayout = new QVBoxLayout(topBox);
    auto calcBtn = new QPushButton(tr("Рассчитать"));
    auto lbl = new QLabel("Заданная точка X");
    _calcLine = new QLineEdit;
    auto returnBut = new QPushButton(tr("Назад"));
    auto tablesBut = new QPushButton(tr("Таблицы"));
    auto tablesAllValuesBut = new QPushButton(tr("Таблицы"));
    auto graphicsBut = new QPushButton(tr("Эпюры"));
    returnBut->setFixedSize({100, 50});
    auto botLayout = new QHBoxLayout(botBox);

    botLayout->addWidget(lbl);
    botLayout->addWidget(_calcLine);
    botLayout->addWidget(calcBtn);

    topLayout->addWidget(returnBut);
    topLayout->addWidget(tablesBut);
    topLayout->addWidget(tablesAllValuesBut);
    topLayout->addWidget(graphicsBut);
    mainLayout->addWidget(topBox);
    mainLayout->addWidget(botBox);
    connect(returnBut, &QPushButton::clicked, this, &ResultWindow::returnToTables);
    connect(tablesBut, &QPushButton::clicked, this, &ResultWindow::showTables);
    connect(tablesAllValuesBut, &QPushButton::clicked, this, &ResultWindow::showAllValuesTables);
    connect(graphicsBut, &QPushButton::clicked, this, &ResultWindow::showGraphics);
    connect(calcBtn, &QPushButton::clicked, this, &ResultWindow::openPointWindow);
};
void ResultWindow::calcResuluts(const Params& params)
{
    setParams(params);
    clearVectors();
    calcMatrixes(params);   

}
void ResultWindow::showGraphics()
{
    auto dialog = new QDialog{this};
    auto window = new QTabWidget {dialog};
    window->setMinimumSize(1300, 800);

    auto uGraphicWidget = new QWidget(window);
    auto NGraphicWidget = new QWidget(window);

    window->insertTab(0, NGraphicWidget, tr("График N(x)"));
    window->insertTab(1, uGraphicWidget, tr("График u(x)"));
    auto layout_N = new QVBoxLayout {NGraphicWidget};
    auto layout_u = new QVBoxLayout {uGraphicWidget};

    QChart* ch = new QChart();
    auto series = new QLineSeries;
    double x = 0;
    for (auto i = 0; i < _N.size(); ++i)
    {
        for (auto j = 0; j < _N[i].size(); ++j)
        {
            *series << QPointF(x, _N[i][j]);
            x += 0.2;
            if (j == _N[i].size() - 1)
                x -= 0.2;
        }
    }
    ch->addSeries(series);
    ch->createDefaultAxes();
    QChartView* view = new QChartView(ch);
    layout_N->addWidget(view);

    auto ch_u = new QChart();
    auto series_u = new QLineSeries;
    x = 0;
    for (auto i = 0; i < _u.size(); ++i)
    {
        for (auto j = 0; j < _u[i].size(); ++j)
        {
            *series_u << QPointF(x, _u[i][j]);
            x += 0.2;
            if (j == _u[i].size() - 1)
                x -= 0.2;
        }
    }
    ch_u->addSeries(series_u);
    ch_u->createDefaultAxes();
    QChartView* view_u = new QChartView(ch_u);
    layout_u->addWidget(view_u);
    dialog->show();
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
            _b[i] = params.centred_loads[i] + (params.distr_loads[i] * params.type_for_each_rod[i][4])/2
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
        QVector<double> sigmaValues = {_N[i][0]/A, _N[i][_N[i].size()-1]/A};
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
    tablesWindow->setWindowTitle(tr("Краевые значения"));
    tablesWindow->setMinimumSize(1000, 600);
    makeTables(tablesWindow);
    tablesWindow->show();
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

void ResultWindow::defineRod(QTableWidget* tabN, QTableWidget* tabU, QTableWidget* tabS)
{
    double allRodsL = 0;
    for (auto i = 0; i < _params.type_for_each_rod.size(); ++i)
        allRodsL += _params.type_for_each_rod[i][4];
    double value = _calcLine->text().toDouble();
   int rodNumber = getRodNumber(value, allRodsL);
   double rodValue = 0;
   bool checkValue = false;
   for (auto i = 0; i < _params.type_for_each_rod.size(); ++i)
   {
       rodValue += _params.type_for_each_rod[i][4];
       if (value == rodValue)
           checkValue = true;
   }

   if (checkValue == false || value == 0 || value == allRodsL)
       calcOneColumnTables(tabN, tabU, tabS, rodNumber, value);
   else
       calcTwoColumnsTables(tabN, tabU, tabS, rodNumber, value);
}

void ResultWindow::openPointWindow()
{
    if (!checkXValid())
        return;
    auto calcInPointWindow = new QDialog(this, Qt::WindowStaysOnTopHint);
    calcInPointWindow->setWindowTitle(tr("Расчет компонент в сечении X"));
    calcInPointWindow->setMinimumSize(1000, 600);
    auto layout = new QHBoxLayout(calcInPointWindow);
    auto tableN = new QTableWidget(1, 2, calcInPointWindow);
    auto tableU = new QTableWidget(1, 1, calcInPointWindow);
    auto tableSigma = new QTableWidget(1, 2, calcInPointWindow);
    tableN->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("N(X) - стрежень 1")));
    tableN->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Nx(X) - стержень 2")));
    tableU->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Ux(X)")));
    tableSigma->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("σ(X) - стержень 1")));
    tableSigma->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("σ(X) - стержень 2")));
    layout->addWidget(tableN);
    layout->addWidget(tableU);
    layout->addWidget(tableSigma);
    defineRod(tableN, tableU, tableSigma);
    calcInPointWindow->show();
}
void ResultWindow::setParams(const Params& params)
{
    _params.distr_loads = params.distr_loads;
    _params.fixed_nodes = params.fixed_nodes;
    _params.centred_loads = params.centred_loads;
    _params.types_descrip = params.types_descrip;
    _params.types_for_rod = params.types_for_rod;
    _params.type_for_each_rod = params.type_for_each_rod;
}

int ResultWindow::getRodNumber(const double& value, const double& allRodsL)
{
    int rodNumber = 0;
    double coordX = 0;
    for (auto i = 0; i < _params.type_for_each_rod.size(); ++i)
    {
        if (value == allRodsL)
        {
            rodNumber = _params.type_for_each_rod.size() - 1;
            break;
        }
        coordX += _params.type_for_each_rod[i][4];
        if (value >= coordX)
        {
            ++rodNumber;
        }
        else
            break;
    }
    return rodNumber;
}

void ResultWindow::calcOneColumnTables(QTableWidget *tabN, QTableWidget *tabU, QTableWidget *tabS, const int &rodNumber, const double& x)
{
    double X = x;
    if (rodNumber != 0 && rodNumber - 1 != 0)
    {
        for (auto i = 0; i < rodNumber; ++i)
            X -= _params.type_for_each_rod[i][4];
    }
    tabN->setColumnCount(1);
    tabS->setColumnCount(1);
    double E = _params.type_for_each_rod[rodNumber][2];
    double A = _params.type_for_each_rod[rodNumber][1];
    double L = _params.type_for_each_rod[rodNumber][4];
    double q = _params.distr_loads[rodNumber];
    double S = _params.type_for_each_rod[rodNumber][3];
    double valueN = ((E * A) / L) * (_U[rodNumber][1] - _U[rodNumber][0]) + ((q * L) / 2) * (1 - 2 * (X / L));
    double valueU = (1 - X/L) * _U[rodNumber][0] + (X / L) * _U[rodNumber][1] + (q * L*L * X)/(2*E*A*L) * (1 - X/L);
    double valueS = valueN / A;
    tabN->setItem(0, 0, new QTableWidgetItem(QString::number(valueN)));
    tabU->setItem(0, 0, new QTableWidgetItem(QString::number(valueU)));
    tabS->setItem(0, 0, new QTableWidgetItem(QString::number(valueS)));
    if (abs(valueS) > abs(S))
        tabS->item(0, 0)->setBackground(Qt::red);
}

void ResultWindow::calcTwoColumnsTables(QTableWidget *tabN, QTableWidget *tabU, QTableWidget *tabS, const int &rodNumber, const double& x)
{
    double X = x;
    double prevX = x;
    if (rodNumber != 0 && rodNumber - 1 != 0)
    {
        for (auto i = 0; i < rodNumber; ++i)
            X -= _params.type_for_each_rod[i][4];
        for (auto i = 0; i < (rodNumber - 1); ++i)
            prevX -= _params.type_for_each_rod[i][4];
    }
    tabN->setColumnCount(2);
    tabS->setColumnCount(2);
    tabS->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("σ(X) - стержень 2")));
    tabN->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Nx(X) - стержень 2")));
    double E = _params.type_for_each_rod[rodNumber][2];
    double A = _params.type_for_each_rod[rodNumber][1];
    double L = _params.type_for_each_rod[rodNumber][4];
    double S = _params.type_for_each_rod[rodNumber][3];
    double q = _params.distr_loads[rodNumber];
    double prevE = _params.type_for_each_rod[rodNumber-1][2];
    double prevA = _params.type_for_each_rod[rodNumber-1][1];
    double prevL = _params.type_for_each_rod[rodNumber-1][4];
    double prevS = _params.type_for_each_rod[rodNumber-1][3];
    double prevq = _params.distr_loads[rodNumber-1];
    double valueN = ((E * A) / L) * (_U[rodNumber][1] - _U[rodNumber][0]) + ((q * L) / 2) * (1 - 2 * (X / L));
    double valueU = (1 - prevX/prevL) * _U[rodNumber-1][0] + (prevX / prevL) * _U[rodNumber-1][1] + (prevq * prevL*prevL * prevX)/(2*prevE*prevA*prevL) * (1 - prevX/prevL);
    double valueS = valueN / A;
    double valuePrevN = ((prevE * prevA) / prevL) * (_U[rodNumber-1][1] - _U[rodNumber-1][0]) + ((prevq * prevL) / 2) * (1 - 2 * (X / prevL));
    double valuePrevS = valuePrevN / prevA;
    tabN->setItem(0, 1, new QTableWidgetItem(QString::number(valueN)));
    tabU->setItem(0, 0, new QTableWidgetItem(QString::number(valueU)));
    tabS->setItem(0, 1, new QTableWidgetItem(QString::number(valueS)));
    tabN->setItem(0, 0, new QTableWidgetItem(QString::number(valuePrevN)));
    tabS->setItem(0, 0, new QTableWidgetItem(QString::number(valuePrevS)));
    if (abs(valuePrevS) > abs(prevS))
        tabS->item(0, 0)->setBackground(Qt::red);
    if (abs(valueS) > abs(S))
        tabS->item(0, 1)->setBackground(Qt::red);
}

bool ResultWindow::checkXValid()
{
    double value = _calcLine->text().toDouble();
    double allRodsL = 0;
    for (auto i = 0; i < _params.type_for_each_rod.size(); ++i)
        allRodsL += _params.type_for_each_rod[i][4];
    if (value < 0 || value > allRodsL)
    {
        QMessageBox::warning(this, tr("Неверные данные"),
                             tr("Значение X не принадлежит стержню"),
                             QMessageBox::Close);
        return false;
    }
    return true;
}

void ResultWindow::makeTables(QDialog *dialog)
{
    auto layout = new QHBoxLayout(dialog);
    auto tableN = new QTableWidget(_N.size(), 2, dialog);
    auto tableU = new QTableWidget(_u.size(), 2, dialog);
    auto tableSigma = new QTableWidget(_u.size(), 2, dialog);
    tableN->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Nx(0)")));
    tableN->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Nx(L)")));
    tableU->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Ux(0)")));
    tableU->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Ux(L)")));
    tableSigma->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("σ(0)")));
    tableSigma->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("σ(L)")));
    layout->addWidget(tableN);
    layout->addWidget(tableU);
    layout->addWidget(tableSigma);
    fill_NUS_tables(tableN, tableU, tableSigma);
    checkSigmaCells(tableSigma);
}
void ResultWindow::fill_NUS_tables(QTableWidget *tableN, QTableWidget *tableU, QTableWidget *tableSigma)
{
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
}
void ResultWindow::showAllValuesTables()
{
    auto tablesWindow = new QDialog(this, Qt::WindowStaysOnTopHint);
    tablesWindow->setWindowTitle(tr("Значения компонент (шаг 0.2)"));
    tablesWindow->setMinimumSize(1000, 600);
    makeTablesAllValues(tablesWindow);
    tablesWindow->show();
}
void ResultWindow::makeTablesAllValues(QDialog *dialog)
{
    auto layout = new QHBoxLayout(dialog);
    if (_N.size() != _u.size())
        return;
    for (auto i = 0; i < _N.size(); ++i)
        if (_N[i].size() != _u[i].size())
            return;
    auto tableN = new QTableWidget(_N.size(), 5, dialog);
    tableN->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Номер стержня")));
    tableN->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("x")));
    tableN->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Nx(x)")));
    tableN->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("ux(x)")));
    tableN->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("σx(x)")));
    layout->addWidget(tableN);

    fill_NUSAllValues_tables(tableN);
    for (auto i = 0; i < tableN->rowCount(); ++i)
        if (abs(tableN->item(i, 4)->text().toDouble()) > abs(_startSigma[tableN->item(i, 0)->text().toInt() - 1]))
            tableN->item(i,4)->setBackground(Qt::red);
}

void ResultWindow::fill_NUSAllValues_tables(QTableWidget *tableN)
{
    int start = 0;
    int rowNumber = 0;
    for (auto i = 0; i < _N.size(); ++i)
        rowNumber += _N[i].size();
    tableN->setRowCount(rowNumber);
    for (auto i = 0; i < _N.size(); ++i)
    {
        double x = 0;
        for (auto j = 0; j < _N[i].size(); ++j)
        {
            tableN->setItem(start, 0, new QTableWidgetItem(QString::number(i+1)));
            tableN->setItem(start, 1, new QTableWidgetItem(QString::number(x)));
            tableN->setItem(start, 2, new QTableWidgetItem(QString::number(_N[i][j])));
            tableN->setItem(start, 3, new QTableWidgetItem(QString::number(_u[i][j])));
            tableN->setItem(start, 4, new QTableWidgetItem(QString::number(_N[i][j]/_params.type_for_each_rod[i][1])));
            x += 0.2;
            ++start;
        }
    }
}

