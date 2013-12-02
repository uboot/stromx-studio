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


#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <QList>
#include <QString>

class QGraphicsItem;
class VisualizationWidget;

class Visualization
{
public:
    Visualization(const QString & visualization, const QString & name)
      : m_visualization(visualization),
        m_name(name)
    {}

    const QString & name() const { return m_name; }
    const QString & visualization() const { return m_visualization; }

    virtual VisualizationWidget* createEditor() const = 0;
    virtual QList<QGraphicsItem*> createItems() const = 0;
    
private:
    QString m_visualization;
    QString m_name;
};

#endif // VISUALIZATION_H