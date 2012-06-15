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

#ifndef ERRORLISTVIEW_H
#define ERRORLISTVIEW_H

#include <QTableView>

class ErrorListModel;

/** \brief View which displays a list of errors. */
class ErrorListView : public QTableView
{
    Q_OBJECT

public:
    /** Constructs an error list view. */
    explicit ErrorListView(QWidget *parent = 0);
    
    /** Returns the model of the view. */
    ErrorListModel* errorListModel() const { return m_model; }
    
private:
    ErrorListModel* m_model;
};

#endif // ERRORLISTVIEW_H
