/* 
*  Copyright 2011 Matthias Fuchs
*
*  This file is part of stromx-studio.
*
*  Stromx-studio is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Stromx-studio is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with stromx-studio.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include <stromx/runtime/Exception.h>
#include <fstream>
#include "widget/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/icon.png"));
    
    // look here for a transparent splash screen without background:
    // http://developer.qt.nokia.com/wiki/Custom_splashscreen_with_text
    QPixmap pixmap(":/images/stromx.png");
    QSplashScreen splash(pixmap);
    splash.show();
    a.processEvents();
    
    MainWindow w;
    w.show();

    try
    {
        return a.exec();
    }
    catch(stromx::runtime::Exception &e)
    {
        std::ofstream file("callstack.txt");
        file << e.callStack();
    }
}
