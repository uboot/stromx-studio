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

#include <QAbstractItemModel>
#include <QGroupBox>
#include "visualization/VisualizationState.h"

class QCheckBox;
class QComboBox;
class QFormLayout;
class VisualizationWidget;

/** 
 * \brief Widget for the properties of an observed input
 * 
 * This widget contains controls to display and edit an observed input in the
 * observer list view. It is used by the item delegate of the observer list
 * view (InputDelegate).
 */
class InputEditWidget : public QGroupBox
{
    Q_OBJECT
    
public:
    InputEditWidget(const QModelIndex & index, QWidget* parent = 0);
    
    const VisualizationState & state() const { return m_state; }
    void setState(const VisualizationState & state);
    void setInputTitle(const QString & title);
    
signals:
    void dataChanged();
    
private slots:
    void handleModelChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
    void updateState();
    void updateWidget();
    
private:    
    QModelIndex m_index;
    VisualizationState m_state;
    VisualizationWidget* m_widget;
    QCheckBox* m_activeCheckBox;
    QComboBox* m_visualizationMenu;
    QFormLayout* m_layout;
    QGroupBox* m_propertiesGroupBox;
    bool m_isSettingState;
};

#endif // INPUTEDITWIDGET_H
