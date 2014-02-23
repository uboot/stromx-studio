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


#ifndef VISUALIZATIONWIDGET_H
#define VISUALIZATIONWIDGET_H

#include <QWidget>

/**
 * \brief Abstract base class of widget to set visualization properties
 * 
 * Visualization widgets are displayed as editors in the list of input models
 * in an observer window.
 */
class VisualizationWidget : public QWidget
{
    Q_OBJECT
    
public:
    /** Constructs a visualization widget. */
    explicit VisualizationWidget(QWidget* parent = 0);
    
    /**
     * Queries the current settings from the controls in the widget and returns 
     * them as property map.
     */
    virtual QMap<QString, QVariant> getProperties() const = 0;
    
    /** Sets the controls of widget to the input properties. */
    virtual void setProperties(const QMap<QString, QVariant> & properties) = 0;
     
signals:
    /** 
     * A value of a control in the widget has changed. This results in an undo
     * stack command which sets the current visualization properties of the input
     * model.
     */
    void valueChanged();
};


#endif // VISUALIZATIONWIDGET_H