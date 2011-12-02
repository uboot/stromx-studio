/* 
*  Copyright 2011 Matthias Fuchs
*
*  This file is part of stromx-gui.
*
*  Stromx-gui is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Stromx-gui is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with stromx-gui.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
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
