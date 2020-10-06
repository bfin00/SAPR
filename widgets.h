#include <QPixmap>
#include <QVector>
#include <QWidget>
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
class QGroupBox;
class QLineEdit;
class Widgets : public QWidget
{
public:
    explicit Widgets(QWidget *parent = nullptr);
    Model *model;
private:
    void setupWidgets();
    QGroupBox *horizontalGroupBox;
    QLineEdit *lineEdit;
    QGroupBox *box1;
    QGroupBox *box2;
    QTableWidget *t_top, *t_1, *t_2, *t_3, *t_4, *t_5, *t_6;
    QLineEdit *line1, *line2, *line3;
    
    void setTab(int s);
};

#endif // WIDGETS_H
