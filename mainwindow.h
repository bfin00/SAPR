#pragma once

#include <QMainWindow>
class RenderArea;
struct Params;
class Tables;
class QGroupBox;
class QVBoxLayout;
class QStackedWidget;
class ResultWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:


private:
    Tables* _tables{};
    void setUpMenus();
    void setTableWindow();
    void setResultWindow();
    QGroupBox* box;    // отрисовка и данные таблиц
    QVBoxLayout* mainLayout;
    RenderArea* _area;
    ResultWindow* _resultWindow;
    
    QWidget* results;
    
    QStackedWidget* stackedWidget;
};

