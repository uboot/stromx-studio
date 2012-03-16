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

class AbstractDataVisualizer
{
public:
    virtual void addLayer(int pos) = 0;
    virtual void moveLayer(int src, int dest) = 0;
    virtual void removeLayer(int pos) = 0;
    virtual void setColor(int layer, const QColor & color) = 0;
    virtual void setAlpha(int layer, int alpha) = 0;
    virtual void setData(int layer, stromx::core::Data& data) = 0;
    virtual void setActive(int layer, bool active) = 0;
    
private:
    
};

#endif // ABSTRACTDATAVISUALIZER_H
