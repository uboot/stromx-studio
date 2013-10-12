/* 
*  Copyright 2013 Matthias Fuchs
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

#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include <QGroupBox>

#include "AbstractDataVisualizer.h"


/** 
 * \brief Widget for the properties of an observed input
 * 
 * This widget contains controls to display and edit an observed input in the
 * observer list view. It is used by the item delegate of the observer list
 * view (InputDelegate).
 */
class InputWidget : public QGroupBox
{
    Q_OBJECT
    
public:
    InputWidget(QWidget* parent = 0);
    
    void setInputTitle(const QString & title);
    void setInputColor(const QColor & color);
    void setInputActive(const bool isActive);
    void setVisualizationType(const int type);
    
private:
};

#endif // INPUTWIDGET_H
