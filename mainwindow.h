#pragma once

#include <QMainWindow>

struct Params;
class Tables;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void calcResult(const Params& params);

private:
    Tables* _tables{};
};
