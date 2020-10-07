#include "widgets.h"
#include <QtWidgets>
#include <QTableWidgetItem>
Widgets::Widgets(QWidget *parent) : QWidget(parent)
{

    setupWidgets();
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(box1);
    main_layout->addWidget(box2);
    setLayout(main_layout);
}
void Widgets::setupWidgets()
{
    model = new Model(this);
    box1 = new QGroupBox(tr("Введите данные здесь"), this);
    QHBoxLayout *top_l = new QHBoxLayout(box1);
    QPushButton *but = new QPushButton(tr("Сохранить"));
    QLabel *lbl1 = new QLabel(tr("Количество\nстержней"));
    line1 = new QLineEdit;
    QLabel *lbl2 = new QLabel(tr("Количество\nтипов стержней"));
    line2 = new QLineEdit;
    QLabel *lbl3 = new QLabel(tr("Количество\nузлов"));
    line3 = new QLineEdit;
    QLabel *lbl4 = new QLabel(tr("Сосредоточенные\nнагрузки"));
    line4 = new QLineEdit;
    QLabel *lbl5 = new QLabel(tr("Продальные\nнагрузки"));
    line5 = new QLineEdit;
    top_l->addWidget(lbl1);
    top_l->addWidget(line1);
    top_l->addWidget(lbl2);
    top_l->addWidget(line2);
    top_l->addWidget(lbl3);
    top_l->addWidget(line3);
    top_l->addWidget(lbl4);
    top_l->addWidget(line4);
    top_l->addWidget(lbl5);
    top_l->addWidget(line5);
    top_l->addWidget(but);

    box2 = new QGroupBox(this);
    QHBoxLayout *bot_l = new QHBoxLayout(box2);
    t_1 = new QTableWidget(1, 1, this);
    t_2 = new QTableWidget(1, 1, this);
    t_3 = new QTableWidget(1, 4, this);
    t_4 = new QTableWidget(2, 1, this);
    t_5 = new QTableWidget(1, 2, this);
    t_6 = new QTableWidget(1, 2, this);

    t_1->setMaximumSize(160, 200);
    t_2->setMaximumSize(160, 200);
    t_3->setMaximumSize(640, 200);
    t_4->setMaximumSize(160, 200);
    t_5->setMaximumSize(160, 200);
    t_6->setMaximumSize(160, 200);
    bot_l->addWidget(t_1);
    bot_l->addWidget(t_2);
    bot_l->addWidget(t_3);
    bot_l->addWidget(t_4);
    bot_l->addWidget(t_5);
    bot_l->addWidget(t_6);

    connect(but, &QPushButton::clicked, this, &Widgets::setUpTables);
    connect(t_1, &QTableWidget::cellChanged, this, &Widgets::addValuesToV);
}

void Widgets::setUpTables()
{
    t_2->setRowCount(line1->text().toInt());
    t_3->setRowCount(line2->text().toInt());
    t_1->setRowCount(line3->text().toInt());
    t_5->setRowCount(line4->text().toInt());
    t_6->setRowCount(line5->text().toInt());
}
void Widgets::addValuesToV(int r, int c)
{

}

