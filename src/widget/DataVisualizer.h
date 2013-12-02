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

#ifndef DATAVISUALIZER_H
#define DATAVISUALIZER_H

#include <QMap>

#include <stromx/runtime/Data.h>

#include "AbstractDataVisualizer.h"
#include "GraphicsView.h"

/** 
 * \brief Data visualizer based on QGraphicsView
 * 
 * This implementation of an AbstractDataVisualizer is derived from QGraphicsView
 * and uses QGraphicsItem objects to display data. The position of a layer translates
 * into the z-value of the graphic items.
 */
class DataVisualizer : public GraphicsView, public AbstractDataVisualizer
{
public:
    DataVisualizer(QWidget* parent = 0);
    
    virtual void addLayer(int pos);
    virtual void moveLayer(int src, int dest);
    virtual void removeLayer(int pos);
    virtual void setColor(int pos, const QColor & color);
    virtual void setData(int pos, const stromx::runtime::Data& data,
                         const VisualizationState & state);
    virtual void setActive(int pos, bool active);
    
private:
    /** Maps each existing layer to a list of graphic items the layer contains. */
    QMap<int, QList<QGraphicsItem*> > m_items;
};

#endif // DATAVISUALIZER_H
