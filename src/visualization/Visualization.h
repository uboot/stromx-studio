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

#include "VisualizationState.h"

namespace stromx
{
    namespace runtime
    {
        class Data;
    }
}

class QGraphicsItem;
class VisualizationWidget;

/** 
 * \brief Abstract visualization
 * 
 * Classes derived from this class represent a \em visualization of the certain
 * stromx data types. A visualization converts stromx data to QGraphicsItem objects.
 * A visualization has properties which control the conversion. It must provide
 * a visualization widget which allows the user to set these properties.
 */
class Visualization
{
public:
    /** Constructs a visualization. */
    Visualization(const QString & visualization, const QString & name)
      : m_visualization(visualization),
        m_name(name)
    {}
    virtual ~Visualization() {}

    /** Returns the user-readable name of the visualization. */
    const QString & name() const { return m_name; }
    
    /** Returns the unique identfier of the visualization. */
    const QString & visualization() const { return m_visualization; }

    /** Returns the editor to be displayed in the input list. */
    virtual VisualizationWidget* createEditor() const = 0;
    
    /** 
     * Creates graphics items for the given input data and the given 
     * visualization properties.
     */
    virtual QList<QGraphicsItem*> createItems(const stromx::runtime::Data & data,
        const VisualizationState::Properties & properties) const = 0;
    
private:
    QString m_visualization;
    QString m_name;
};

#endif // VISUALIZATION_H