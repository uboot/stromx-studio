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

#ifndef OBSERVERVIEW_H
#define OBSERVERVIEW_H

#include <QTableView>

class ObserverModel;
/**
 * \brief Table view which displays an observer model.
 * 
 * The observer view displays the contents of an observer model, i.e. a list
 * of inputs and their properties, in a table view.
 */
class ObserverView : public QTableView
{
    Q_OBJECT
    
public:
    /** Constructs an observer view from an observer model. */
    explicit ObserverView(ObserverModel* observer, QWidget* parent = 0);
    
    /** Returns the observer model of the view. */
    ObserverModel* model() const { return m_observer; }
    
private:
    ObserverModel* m_observer;
};

#endif // OBSERVERVIEW_H