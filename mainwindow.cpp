#include "mainwindow.h"
#include "Tables.h"

#include <QtWidgets>
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{

    _tables = new Tables{ this };
    setUpMenus();
    connect(_tables, &Tables::passParams, this, &MainWindow::calcResult);
    setWindowTitle(QStringLiteral("САПР"));
    setCentralWidget(_tables);
    showMaximized();
}

void MainWindow::setUpMenus()
{
    auto fileMenu = menuBar()->addMenu(tr("File"));

    auto *saveAct = new QAction(tr("Save") ,this);
    fileMenu->addAction(saveAct);
    connect(saveAct, &QAction::triggered, _tables, &Tables::save);
    auto *openAct = new QAction(tr("Open"), this);
    fileMenu->addAction(openAct);
    connect(openAct, &QAction::triggered, _tables, &Tables::read);
    auto exitAction = fileMenu->addAction(tr("E&xit"), qApp, &QCoreApplication::quit);
    exitAction->setShortcuts(QKeySequence::Quit);

}
MainWindow::~MainWindow() = default;

void MainWindow::calcResult(const Params& params)
{
//обработка результатов(возможно, вынести в отдельный класс)
}

