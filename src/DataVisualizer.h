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

#include <QGraphicsView>
#include <QMap>

#include "AbstractDataVisualizer.h"

class DataVisualizer : public QGraphicsView, public AbstractDataVisualizer
{
public:
    DataVisualizer(QWidget* parent = 0);
    
    virtual void addLayer(int layer);
    virtual void moveLayer(int src, int dest);
    virtual void removeLayer(int layer);
    virtual void setColor(int layer, const QColor & color);
    virtual void setAlpha(int layer, int alpha);
    virtual void setData(int layer, const stromx::core::Data& data);
    virtual void setActive(int layer, bool active);
    
private:
    QMap<int, QGraphicsItem*> m_items;
};

#endif // DATAVISUALIZER_H
