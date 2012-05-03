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

namespace stromx
{
    namespace core
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
 * 
 * In addition to the z-value each layer has several properties such as color and
 * alpha value wich can influence the way how data in the layer is displayed. 
 * These properties of a layer (including the position) are persistent even if the 
 * data of the layer changes.
 */
class AbstractDataVisualizer
{
public:
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
     * Sets the color of the objects in the layer at \c pos to \c color. 
     * If no layer exists at \c src the function does not do anything.
     */
    virtual void setColor(int pos, const QColor & color) = 0;
    
    /** 
     * Sets the alpha value of the objects in the layer at \c pos to \c alpha.
     * If no layer exists at \c src the function does not do anything.
     */
    virtual void setAlpha(int pos, int alpha) = 0;
    
    /** 
     * Sets the data in the layer at \c pos to \c data. If no layer exists
     * at \c pos the function does not do anything.
     */
    virtual void setData(int pos, const stromx::core::Data& data) = 0;
    
    /**
     * Sets the layer at \c pos to be active or inactive, i.e. it is visible or 
     * invisible. If no layer exists at \c src the function does not do anything.
     */
    virtual void setActive(int pos, bool active) = 0;
    
private:
    
};

#endif // ABSTRACTDATAVISUALIZER_H
