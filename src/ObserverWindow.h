/* 
*  Copyright 2012 Matthias Fuchs
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

#ifndef OBSERVERWINDOW_H
#define OBSERVERWINDOW_H

#include <QWidget>

class ObserverModel;
class ObserverTreeModel;

class ObserverWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit ObserverWindow(ObserverModel* observer, QWidget* parent);
    
    ObserverModel* observer() const { return m_observer; }
    QAction* showAction() const { return m_showAct; }
    
private slots:
    void updateActionText(const QString & name);
    
private:
    ObserverModel* m_observer;
    QAction* m_showAct;
};

#endif // OBSERVERWINDOW_H