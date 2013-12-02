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


#ifndef DEFAULTVISUALIZATION_H
#define DEFAULTVISUALIZATION_H

#include <QObject>
#include "visualization/Visualization.h"

class QComboBox;

class DefaultVisualization : public Visualization
{
public:
    DefaultVisualization() : Visualization("default", QObject::tr("Default")) {}

    virtual VisualizationWidget* createEditor() const;
    virtual QList<QGraphicsItem*> createItems(const stromx::runtime::Data & data,
        const VisualizationState::Properties & properties) const;
};

#endif // DEFAULTVISUALIZATION_H