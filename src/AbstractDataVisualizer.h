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

#ifndef ABSTRACTDATAVISUALIZER_H
#define ABSTRACTDATAVISUALIZER_H

#include <QMap>
#include <QVariant>

#include "VisualizationState.h"

namespace stromx
{
    namespace runtime
    {
        class Data;
    }
}

class QColor;

/**
 * \brief Abstract data visualizer
 * 
 * This class is an abstract interface of a visualizing widget which displays
 * layers of data. Each layer has a z-position can contain at most one data object. 
 * The data is displayed according to the \em position of its layer, i.e. layers with
 * smaller position values are displayed on top of layers with larger positions.
 */
class AbstractDataVisualizer
{
public:
    /** Determines how data is interpreted for visualization. */
    enum VisualizationType
    {
        /** 
         * The visualization is automatically determined from the data type. 
         * This will work only for specific data types such as images. */
        AUTOMATIC,
        
        /** If possible the data is visualized as a set of lines or line segments.*/
        LINES,
        
        /** If possible the data is visualized as a set of points.*/
        POINTS,
        
        
        /** If possible the data is visualized as a histogram. */
        HISTOGRAM,
        
        /** If possible the data is visualized as an image although 
         *it is not explicitly of type image (e.g. a matrix of floats). */
        IMAGE
    };
    
    /** 
     * Adds a layer to the visualizer at the position \c pos. Does not do
     * anything if the layer exists.
     */
    virtual void addLayer(int pos) = 0;
    
    /**
     * Moves the layer at the position \c src to \c dest. If a layer exists
     * at \c dest it is overwritten. If no layer exists at \c src the function
     * does not do anything.
     */
    virtual void moveLayer(int src, int dest) = 0;
    
    /** If a layer exists at \c pos it is removed. */
    virtual void removeLayer(int pos) = 0;
    
    /** 
     * Sets the data in the layer at \c pos to \c data. Any other data is
     * automatically removed from the layer. The data is visualized as 
     * defined by the parameter \c visualizationProperties.
     * If no layer exists at \c pos the function does not do anything.
     * Properties such as alpha value and color of the layer are reset by
     * this function and must be set again for the new data object.
     */
    virtual void setData(int pos, const stromx::runtime::Data& data,
                         const VisualizationState & visualizationState) = 0;
    
private:
    
};

#endif // ABSTRACTDATAVISUALIZER_H
