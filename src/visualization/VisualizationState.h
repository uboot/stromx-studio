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


#ifndef VISUALIZATIONSTATE_H
#define VISUALIZATIONSTATE_H

#include <QVariant>

class VisualizationState
{
    friend QDataStream & operator<< (QDataStream & stream, const VisualizationState & state);
    friend QDataStream & operator>> (QDataStream & stream, VisualizationState & state);
    friend bool operator==(const VisualizationState & lhs, const VisualizationState & rhs);

public:
    typedef QMap<QString, QVariant> Properties;
    
    VisualizationState();
    VisualizationState(const VisualizationState & state);
    virtual ~VisualizationState() {}
    
    bool isActive() const { return m_active; }
    QString currentVisualization() const { return m_visualization; }
    
    void setIsActive(const bool active);
    void setCurrentVisualization(const QString & visualization);
    
    Properties & properties(const QString & visualization);
    const Properties properties(const QString & visualization) const;
    Properties & currentProperties();
    const Properties currentProperties() const;
    
    const VisualizationState & operator=(const VisualizationState & state);
    
private:
    bool m_active;
    QString m_visualization;
    QMap<QString, Properties > m_properties;
};

Q_DECLARE_METATYPE(VisualizationState)

QDataStream & operator<< (QDataStream & stream, const VisualizationState & state);
QDataStream & operator>> (QDataStream & stream, VisualizationState & state);
bool operator==(const VisualizationState & lhs, const VisualizationState & rhs);
bool operator!=(const VisualizationState & lhs, const VisualizationState & rhs);

#endif // VISUALIZATIONSTATE_H