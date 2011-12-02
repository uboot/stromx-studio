#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
     createActions();
     createMenus();
     createToolBars();
     createStatusBar();
     createDockWindows();

     setWindowTitle(tr("Stromx GUI"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::createActions()
{
}

void MainWindow::createDockWindows()
{
}

void MainWindow::createMenus()
{
}

void MainWindow::createStatusBar()
{
}

void MainWindow::createToolBars()
{
}
