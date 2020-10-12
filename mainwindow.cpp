#include "mainwindow.h"
#include "Tables.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    _tables = new Tables{ this };
    connect(_tables, &Tables::passParams, this, &MainWindow::calcResult);
    setWindowTitle(QStringLiteral("САПР"));
    setCentralWidget(_tables);
    showMaximized();
}

MainWindow::~MainWindow() = default;

void MainWindow::calcResult(const Params& params)
{
//обработка результатов(возможно, вынести в отдельный класс)
}

