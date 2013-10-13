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

#ifndef INPUTEDITWIDGET_H
#define INPUTEDITWIDGET_H

#include "widget/InputWidget.h"

class QCheckBox;
class QComboBox;

/** 
 * \brief Widget for the properties of an observed input
 * 
 * This widget contains controls to display and edit an observed input in the
 * observer list view. It is used by the item delegate of the observer list
 * view (InputDelegate).
 */
class InputEditWidget : public InputWidget
{
    Q_OBJECT
    
public:
    InputEditWidget(QWidget* parent = 0);
    
    const QColor inputColor() const;
    bool inputActive() const;
    int visualizationType() const;
    
    void setInputTitle(const QString & title);
    void setInputColor(const QColor & color);
    void setInputActive(const bool isActive);
    void setVisualizationType(const int type);
    
signals:
    void dataChanged();
    
private:    
    QCheckBox* m_activeCheckBox;
    QComboBox* m_colorComboBox;
    QComboBox* m_visualizationTypeComboBox;
};

#endif // INPUTEDITWIDGET_H
