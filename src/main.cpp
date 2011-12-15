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

#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // look here for a transparent splash screen without background:
    // http://developer.qt.nokia.com/wiki/Custom_splashscreen_with_text
    QPixmap pixmap(":/images/stromx.png");
    QSplashScreen splash(pixmap);
    splash.show();
    a.processEvents();
    
    MainWindow w;
    w.show();

    return a.exec();
}
