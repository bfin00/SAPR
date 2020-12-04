#include "Tables.h"

#include <QtWidgets>
#include <QLineEdit>
#include <QGroupBox>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "mainwindow.h"
//Сделать: выход из функции при некорректных данных
//         вынос в лямбда функции эмитов сигналов непосредственно в коннекте
Tables::Tables(QWidget *parent)
    : QWidget(parent)
{
    createTopBox();
    createTableWidgets();

    _calcBtn = new QPushButton{ QStringLiteral("Рассчитать"), this };
    connect(_calcBtn, &QPushButton::clicked, this, &Tables::prepareParams);

    auto btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(_calcBtn);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(_box1);
    mainLayout->addWidget(_box2);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);
}

void Tables::setParamTables()
{
    if (!checkLines())
        return;

    _t_2->setRowCount(_line1->text().toInt());
    _t_3->setRowCount(_line2->text().toInt());
    //t_4 - задается явно
    _t_5->setRowCount(_line3->text().toInt());
    _t_6->setRowCount(_line1->text().toInt());
}

void Tables::prepareParams()
{
    if(!checkTableEmptyness())
        return;
    saveParams();

    emit passParams(params);
}

void Tables::saveParams()
{
    if (!checkRods())
        return;
    clearVectors(params);
    checkEmptyCells(_t_2);
    checkEmptyCells(_t_3);
    checkEmptyCells(_t_5);
    checkEmptyCells(_t_6);

    addValuesToV(_t_2, params.types_for_rod);
    addValuesToV(_t_3, params.types_descrip);
    addValuesToV(_t_4, params.fixed_nodes);
    addValuesToV(_t_5, params.centred_loads);
    addValuesToV(_t_6, params.distr_loads);
//    convertLoads(params.distr_loads);
//    convertLoads(params.centred_loads);
    params.type_for_each_rod.resize(params.types_for_rod.size());
    for (auto i = 0; i < params.types_for_rod.size(); ++i)
    {
        for (auto j = 0; j < 5; ++j)
        {
            params.type_for_each_rod[i].resize(5);
            if (j == 0)
            {
                params.type_for_each_rod[i][j] = i+1;
            }
            else
            {
                params.type_for_each_rod[i][j] = params.types_descrip[params.types_for_rod[i] - 1][j-1];
            }
        }
    }
    if (!checkFixedNodes())
        return;
}

void Tables::convertLoads(QVector<int>& vec)
{
    for (auto i = 0; i < vec.size(); ++i)
    {
        vec[i] = vec[i] * 100;
    }
}

void Tables::createTopBox()
{
    _box1 = new QGroupBox(QStringLiteral("Введите данные здесь"), this);
    auto inputLayout = new QHBoxLayout;
    auto but = new QPushButton(QStringLiteral("Создать таблицы"));
    connect(but, &QPushButton::clicked, this, &Tables::setParamTables);
    QLabel* lbl1 = new QLabel(QStringLiteral("Количество\nстержней"));
    _line1 = new QLineEdit{ this };
    QLabel* lbl2 = new QLabel(QStringLiteral("Количество\nтипов стержней"), this);
    _line2 = new QLineEdit{ this };
    QLabel* lbl3 = new QLabel(QStringLiteral("Количество\nузлов"), this);
    _line3 = new QLineEdit{ this };

    inputLayout->addWidget(lbl1);
    inputLayout->addWidget(_line1);
    inputLayout->addWidget(lbl2);
    inputLayout->addWidget(_line2);
    inputLayout->addWidget(lbl3);
    inputLayout->addWidget(_line3);
    inputLayout->addWidget(but);
    _box1->setLayout(inputLayout);

    QRegularExpression re{ "[1-9][0-9][0-9]" };
    auto val = new QRegularExpressionValidator{ re, this };
    _line1->setValidator(val);
    _line2->setValidator(val);
    _line3->setValidator(val);
}

void Tables::createTableWidgets()
{
    auto drowBut = new QPushButton(tr("Отрисовать конструкцию"));
    connect(drowBut, &QPushButton::clicked, this, &Tables::saveParams);
    connect(drowBut, &QPushButton::clicked, this, &Tables::sendParamsForDraw); // возможно как-то вынести в лямбда-функцию
    _box2 = new QGroupBox(QStringLiteral("Параметры стержня"), this);
    auto tableLayout = new QHBoxLayout{};
    _t_2 = new QTableWidget(0, 1, this);
    _t_3 = new QTableWidget(0, 4, this);
    _t_4 = new QTableWidget(2, 1, this);
    _t_4->setItem(0, 0, new QTableWidgetItem("0"));
    _t_4->setItem(1, 0, new QTableWidgetItem("0"));
    _t_5 = new QTableWidget(0, 1, this);
    _t_6 = new QTableWidget(0, 1, this);

    left = new QCheckBox(tr("Заделка слева"));
    right = new QCheckBox(tr("Заделка справа"));
    connect(left, &QCheckBox::stateChanged, this, &Tables::checkBoxForLeft);
    connect(right, &QCheckBox::stateChanged, this, &Tables::checkBoxForRight);

    setWidgetSize();
    setHeaders();

    tableLayout->addWidget(_t_2);
    tableLayout->addWidget(_t_3);
    //tableLayout->addWidget(_t_4);
    tableLayout->addWidget(left);
    tableLayout->addWidget(right);
    tableLayout->addWidget(_t_5);
    tableLayout->addWidget(_t_6);
    tableLayout->addWidget(drowBut);
    _box2->setLayout(tableLayout);
}

void Tables::clearVectors(Params& params)
{
    params.distr_loads.clear();
    params.fixed_nodes.clear();
    params.centred_loads.clear();
    params.types_descrip.clear();
    params.types_for_rod.clear();
}

template <typename T>
void Tables::addValuesToV(QTableWidget* table, QVector<QVector<T>>& vector)
{
    for (auto i = 0; i < table->rowCount(); ++i)
    {
        vector.resize(table->rowCount());
        for (auto j = 0; j < table->columnCount(); ++j)
        {
            vector[i].resize(table->columnCount());
            vector[i][j] = table->item(i, j)->text().toDouble();
        }
    }
}
template <typename T>
void Tables::addValuesToV(QTableWidget* table, QVector<T>& vector)
{
    for (auto i = 0; i < table->rowCount(); ++i)
           for (auto j = 0; j < table->columnCount(); ++j)
               vector.append(table->item(i, j)->text().toInt());
}

void Tables::setHeaders()
{
    _t_2->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Тип стержня")));
    _t_3->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("A")));
    _t_3->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("E")));
    _t_3->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("σ")));
    _t_3->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("L")));
    //_t_4->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Заделки")));
    _t_5->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Сосредоточенные\nсилы")));
    _t_6->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Продольные\nсилы")));
}

void Tables::setWidgetSize()
{
    _t_2->setMaximumSize(160, 200);
    _t_3->setMaximumSize(640, 200);
    _t_4->setMaximumSize(0, 0); //переделать потом
    _t_5->setMaximumSize(160, 200);
    _t_6->setMaximumSize(160, 200);
}

bool Tables::checkLines()
{
    if (_line2->text().toInt() > _line1->text().toInt())
    {
        QMessageBox::warning(this, tr("Неверные данные"),
                             tr("Количество типов стержней не может быть больше количества стержней"),
                             QMessageBox::Close);
        return false;
    }
    if (_line1->text() == "\0" || _line2->text() == "\0" || _line3->text() == "\0")
    {
        QMessageBox::warning(this, tr("Пустое поле"),
                             tr("Не введено поле"),
                             QMessageBox::Close);
        return false;
    }
    if (_line3->text().toInt() != _line1->text().toInt() + 1)
    {
        QMessageBox::warning(this, tr("Неверные данные"),
                             tr("Количество узлов должно быть больше на 1 количества стержней"),
                             QMessageBox::Close);
        return false;
    }
    return true;
}

bool Tables::save()
{
    QFileDialog dialog(this);
        dialog.setWindowModality(Qt::WindowModal);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        if (dialog.exec() != QDialog::Accepted)
            return false;
        return saveFile(dialog.selectedFiles().first());

}

bool Tables::saveFile(const QString& fileName)
{
    QString errorMessage;

        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
        QSaveFile file(fileName);
        if (file.open(QFile::WriteOnly | QFile::Text))
        {
            QTextStream out(&file);

            out << tr("Тип для стержня") << endl;
            writeToFile(out, params.types_for_rod);
            out << tr("Описание типов стержня") << endl;
            writeToFile(out, params.types_descrip);
            out << tr("Заделки") << endl;
            writeToFile(out, params.fixed_nodes);
            out << tr("Сосредоточенные нагрузки") << endl;
            writeToFile(out, params.centred_loads);
            out << tr("Продольные нагрузки") << endl;
            writeToFile(out, params.distr_loads);

            if (!file.commit())
            {
                errorMessage = tr("Cannot write file %1:\n%2.")
                               .arg(QDir::toNativeSeparators(fileName), file.errorString());
            }
        }
        else
        {
            errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
        QGuiApplication::restoreOverrideCursor();

        if (!errorMessage.isEmpty())
        {
            QMessageBox::warning(this, tr("Application"), errorMessage);
            return false;
        }

        return true;
}

void Tables::writeToFile(QTextStream &out, const QVector<QVector<double>> &vec)
{
    for (auto i = 0; i < vec.size(); ++i)
    {
        for (auto j = 0; j < vec.at(i).size(); ++j)
        {
            out << vec[i][j] << " ";
        }
        out << endl;
    }
    out << endl;
}
template <typename T>
void Tables::writeToFile(QTextStream &out, const QVector<T> &vec)
{
    for (auto i = 0; i < vec.size(); ++i)
    {
        out << vec[i] << " ";
    }
    out << endl << endl;
}
bool Tables::checkFixedNodes()
{
    if (params.fixed_nodes[0] > _line3->text().toInt() || params.fixed_nodes[1] > _line3->text().toInt())
    {
        QMessageBox::warning(this, tr("Неверные данные"),
                             tr("неверный номер узла"),
                             QMessageBox::Close);
        return false;
    }
    if (params.fixed_nodes[0] == params.fixed_nodes[1])
    {
        QMessageBox::warning(this, tr("Неверные данные"),
                             tr("совпадают номера узлов"),
                             QMessageBox::Close);
        return false;
    }
    return true;
}
bool Tables::checkRods()
{
    for (auto i = 0; i < _t_2->rowCount(); ++i)
    {
        if (_t_2->item(i, 0)->text().toInt() > _line2->text().toInt() || _t_2->item(i, 0)->text().toInt() < 1)
        {
            QMessageBox::warning(this, tr("Неверные данные"),
                                 tr("неверный тип стержня"),
                                 QMessageBox::Close);
            return false;
        }
    }
    return true;
}
void Tables::read()
{
    QString fileName;
    QVector<QString> ss;
    fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "C:\\Users\\Admin\\Desktop", tr("Text (*.txt)"));
    readFromFile(fileName);
}
void Tables::readFromFile(const QString& fileName)
{
    clearVectors(params);
    QVector<QString> strsFromFile;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
    QTextStream in(&file);
    readIntoVecs(in, params.types_for_rod);
    readIntoVecs(in, params.types_descrip);
    readIntoVecs(in, params.fixed_nodes);
    readIntoVecs(in, params.centred_loads);
    readIntoVecs(in, params.distr_loads);

    setTables();
}

void Tables::readIntoVecs(QTextStream& in, QVector<int>& vec)
{
    in.readLine();
    QString s_1 = in.readLine();
    QStringList l_1 = s_1.split(" ");
    l_1.removeLast();  // удаляю последний элемент, так как при записи в файл ставится
                       //  пробел после последнего элемента, что ведет к элементу "" в списке в конце
    for (auto j = 0; j < l_1.size(); ++j)
        vec.push_back(l_1[j].toInt());

    in.readLine();
}
void Tables::readIntoVecs(QTextStream& in, QVector<QVector<double>>& vec)
{
    in.readLine();

    QString s_2 = in.readLine();
    while (s_2 != "")
    {
        QStringList temp = s_2.split(" ");
        temp.removeLast();
        QVector<double> temp_vec;
        for (auto j = 0; j < temp.size(); ++j)
            temp_vec.push_back(temp[j].toDouble());

        vec.push_back(temp_vec);
        s_2 = in.readLine();
    }
}
void Tables::setTables()
{
    setTable(params.types_for_rod, _t_2);
    setTable(params.types_descrip, _t_3);
    setTable(params.fixed_nodes, _t_4);
    setTable(params.centred_loads, _t_5);
    setTable(params.distr_loads, _t_6);
    _line1->setText(QString::number(params.types_for_rod.size()));
    _line2->setText(QString::number(params.types_descrip.size()));
    _line3->setText(QString::number(params.centred_loads.size()));
    if (params.fixed_nodes[0] != 0)
        left->setCheckState(Qt::Checked);
    if (params.fixed_nodes[1] != 0)
        right->setCheckState(Qt::Checked);
}
void Tables::setTable(const QVector<int> &vec, QTableWidget* table)
{
    table->setRowCount(vec.size());
    for (auto i = 0; i < vec.size(); ++i)
    {
        QString temp = QString::number(vec[i]);
        table->setItem(i, 0, new QTableWidgetItem(temp));
    }
}
void Tables::setTable(const QVector<QVector<double>>& vec, QTableWidget* table)
{
    table->setRowCount(vec.size());
    for (auto i = 0; i < vec.size(); ++i)
    {
        for (auto j = 0; j < vec[i].size(); ++j)
        {
            QString temp = QString::number(vec[i][j]);
            table->setItem(i, j, new QTableWidgetItem(temp));
        }
    }
}

void Tables::drawConstruction()
{
    
}
void Tables::checkBoxForLeft()
{
    if (left->isChecked())
        _t_4->setItem(0, 0, new QTableWidgetItem("1"));
    else
        _t_4->setItem(0, 0, new QTableWidgetItem("0"));
}
void Tables::checkBoxForRight()
{
    if (right->isChecked())
        _t_4->setItem(1, 0, new QTableWidgetItem(_line3->text()));
    else
        _t_4->setItem(1, 0, new QTableWidgetItem("0"));
}

void Tables::sendParamsForDraw()
{
    if(!checkTableEmptyness())
        return;
    emit passParamsForDrawing(params);
}

void Tables::checkEmptyCells(const QTableWidget *t)
{
    for (auto i = 0; i < t->rowCount(); ++i)
    {
        for (auto j = 0; j < t->columnCount(); ++j)
        {
            if (t->item(i, j)->text() == "")
                t->item(i, j)->setText(tr("0"));
        }
    }
}

bool Tables::checkTableEmptyness()
{
    if (_t_2->rowCount() == 0 || _t_3->rowCount() == 0 || _t_5->rowCount() == 0 || _t_6->rowCount() == 0)
    {
        QMessageBox::warning(this, tr("Неверные данные"),
                             tr("Ничего не введено"),
                             QMessageBox::Close);
        return false;
    }
    return true;
}
