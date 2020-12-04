#include "mainwindow.h"
#include "Tables.h"
#include "RenderArea.h"

#include <QtWidgets>
#include "resultwindow.h"
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{

    box = new QGroupBox(this);
    mainLayout = new QVBoxLayout;
    _tables = new Tables{ this };
    _area = new RenderArea();

    stackedWidget = new QStackedWidget(this);

    mainLayout->addWidget(_tables);
    mainLayout->addWidget(_area);
    box->setLayout(mainLayout);
    setUpMenus();

    setWindowTitle(QStringLiteral("САПР"));
    auto _resultWindow = new ResultWindow(this);
    _resultWindow->setMinimumSize(400, 400);
    stackedWidget->addWidget(box);
    stackedWidget->addWidget(_resultWindow);
    stackedWidget->setCurrentIndex(0);
    setCentralWidget(stackedWidget);
    showMaximized();
    connect(_tables, &Tables::passParamsForDrawing, _area, &RenderArea::drawConstruction);
    connect(_tables, &Tables::passParams, _resultWindow, &ResultWindow::calcResuluts);
    connect(_tables, &Tables::passParams, this, &MainWindow::setResultWindow);
    connect(_resultWindow, &ResultWindow::returnBack, this, &MainWindow::setTableWindow);
}

void MainWindow::setUpMenus()
{
    auto fileMenu = menuBar()->addMenu(tr("File"));

    auto *saveAct = new QAction(tr("&Save") ,this);
    fileMenu->addAction(saveAct);
    connect(saveAct, &QAction::triggered, _tables, &Tables::save);
    auto *openAct = new QAction(tr("&Open"), this);
    fileMenu->addAction(openAct);
    connect(openAct, &QAction::triggered, _tables, &Tables::read);
    auto exitAction = fileMenu->addAction(tr("E&xit"), qApp, &QCoreApplication::quit);
    exitAction->setShortcuts(QKeySequence::Quit);

}

void MainWindow::setTableWindow() {stackedWidget->setCurrentIndex(0);}

void MainWindow::setResultWindow(){stackedWidget->setCurrentIndex(1);}

MainWindow::~MainWindow() = default;



