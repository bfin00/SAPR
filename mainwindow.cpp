#include "mainwindow.h"
#include <QtWidgets>
#include <stdlib.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(1800, 900);
    SetupMenus();
    SetupWidgets();
}
void MainWindow::SetupMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("File"));

    QAction *exitAction = fileMenu->addAction(tr("E&xit"), qApp, &QCoreApplication::quit);
    exitAction->setShortcuts(QKeySequence::Quit);
}

void MainWindow::SetupWidgets()
{
    QFrame *frame = new QFrame(this);
    QGridLayout *layout = new QGridLayout(frame);
    w1 = new Widgets(frame);
    m1 = new Model(frame);
    layout->addWidget(w1);
    layout->addWidget(m1);
    setCentralWidget(frame);
}
MainWindow::~MainWindow() {}

