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
    QLabel *lbl1 = new QLabel(tr("Количество стержней"));
    line1 = new QLineEdit;
    QLabel *lbl2 = new QLabel(tr("Количество типов стержней"));
    line2 = new QLineEdit;
    QLabel *lbl3 = new QLabel(tr("Количество узлов"));
    line3 = new QLineEdit;

    top_l->addWidget(lbl1);
    top_l->addWidget(line1);
    top_l->addWidget(lbl2);
    top_l->addWidget(line2);
    top_l->addWidget(lbl3);
    top_l->addWidget(line3);
    top_l->addWidget(but);

    box2 = new QGroupBox(this);
    QHBoxLayout *bot_l = new QHBoxLayout(box2);
    t_1 = new QTableWidget(1, 1, this);
    t_2 = new QTableWidget(1, 1, this);
    t_3 = new QTableWidget(1, 4, this);
    t_4 = new QTableWidget(1, 1, this);
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
    
    connect(line1, SIGNAL(textChanged(const QString)), this, SLOT(setTab(int)));
}
