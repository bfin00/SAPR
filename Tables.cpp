#include "Tables.h"

#include <QtWidgets>
#include <QLineEdit>
#include <QGroupBox>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>

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
    _t_1->setRowCount(_line3->text().toInt());
    _t_2->setRowCount(_line1->text().toInt());
    _t_3->setRowCount(_line2->text().toInt());
    //t_4 - задается явно
    _t_5->setRowCount(_line4->text().toInt());
    _t_6->setRowCount(_line5->text().toInt());
}

void Tables::prepareParams()
{
    Params params;

   clearVectors(params);

   addValuesToV(_t_1, params.x_node);
   addValuesToV(_t_2, params.types_for_rod);
   addValuesToV(_t_3, params.types_descrip);
   addValuesToV(_t_4, params.fixed_nodes);
   addValuesToV(_t_5, params.centred_loads);
   addValuesToV(_t_6, params.distr_loads);

    emit passParams(params);
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
    QLabel* lbl4 = new QLabel(QStringLiteral("Сосредоточенные\nнагрузки"), this);
    _line4 = new QLineEdit{ this };
    QLabel* lbl5 = new QLabel(QStringLiteral("Продольные\nнагрузки"), this);
    _line5 = new QLineEdit{ this };
    inputLayout->addWidget(lbl1);
    inputLayout->addWidget(_line1);
    inputLayout->addWidget(lbl2);
    inputLayout->addWidget(_line2);
    inputLayout->addWidget(lbl3);
    inputLayout->addWidget(_line3);
    inputLayout->addWidget(lbl4);
    inputLayout->addWidget(_line4);
    inputLayout->addWidget(lbl5);
    inputLayout->addWidget(_line5);
    inputLayout->addWidget(but);
    _box1->setLayout(inputLayout);

    QRegularExpression re{ "[1-9][0-9][0-9]" };
    auto val = new QRegularExpressionValidator{ re, this };
    _line1->setValidator(val);
    _line2->setValidator(val);
    _line3->setValidator(val);
    _line4->setValidator(val);
    _line5->setValidator(val);
}

void Tables::createTableWidgets()
{
    _box2 = new QGroupBox(QStringLiteral("Параметры стержня"), this);
    auto tableLayout = new QHBoxLayout{};
    _t_1 = new QTableWidget(0, 1, this);
    _t_2 = new QTableWidget(0, 1, this);
    _t_3 = new QTableWidget(0, 4, this);
    _t_4 = new QTableWidget(2, 1, this);
    _t_5 = new QTableWidget(0, 2, this);
    _t_6 = new QTableWidget(0, 2, this);

    setWidgetSize();
    setHeaders();

    tableLayout->addWidget(_t_1);
    tableLayout->addWidget(_t_2);
    tableLayout->addWidget(_t_3);
    tableLayout->addWidget(_t_4);
    tableLayout->addWidget(_t_5);
    tableLayout->addWidget(_t_6);
    _box2->setLayout(tableLayout);
}

void Tables::clearVectors(Params& params)
{
    params.x_node.clear();
    params.distr_loads.clear();
    params.fixed_nodes.clear();
    params.centred_loads.clear();
    params.types_descrip.clear();
    params.types_for_rod.clear();
}


void Tables::addValuesToV(QTableWidget* table, QVector<QVector<int>>& vector)
{
    for (auto i = 0; i < table->rowCount(); ++i)
    {
        vector.resize(table->rowCount());
        for (auto j = 0; j < table->columnCount(); ++j)
        {
            vector[i].resize(table->columnCount());
            vector[i][j] = table->item(i, j)->text().toInt();
        }
    }
}

void Tables::setHeaders()
{
    _t_1->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("координата X\nдля узла")));
    _t_2->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Тип стержня")));
    _t_3->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("b")));
    _t_3->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("h")));
    _t_3->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("E")));
    _t_3->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("σ")));
    _t_4->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Заделки")));
    _t_5->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Сосредоточенные\nсилы")));
    _t_6->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Продольные\nсилы")));
    _t_5->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("номер узла")));
    _t_6->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("номер стержня")));
}

void Tables::setWidgetSize()
{
    _t_1->setMaximumSize(160, 200);
    _t_2->setMaximumSize(160, 200);
    _t_3->setMaximumSize(640, 200);
    _t_4->setMaximumSize(160, 200);
    _t_5->setMaximumSize(320, 200);
    _t_6->setMaximumSize(320, 200);
}

