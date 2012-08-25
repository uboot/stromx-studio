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

#ifndef INPUTMODEL_H
#define INPUTMODEL_H

#include <QObject>
#include <QColor>

class QUndoStack;
class ObserverTreeModel;
class OperatorModel;

/** 
 * \brief Model of an input observer
 * 
 * This class models observers of single operator inputs. Each input model
 * is associated with an operator input and observes the data arriving at
 * this input. In addition, properties such as color and alpha value of the
 * input model control how this data is visualized..
 */
class InputModel : public QObject
{
    Q_OBJECT
    
    friend class SetInputActiveCmd;
    friend class SetInputColorCmd;
    friend class SetInputVisualizationCmd;
    friend QDataStream & operator>> (QDataStream & stream, ObserverTreeModel * model);
    
public:
    /** Different ways to visualize data which is observed at this input. */
    enum Visualization
    {
        /** 
         * The visualization is automatically determined from the data. 
         * This will work only for specific data types such as images. */
        AUTOMATIC,
        
        /** If possible the observed data is visualized as a set of line segments.*/
        LINE_SEGMENT
    };
    
    /** Constructs an input model for the input \c id of the operator \c op. */
    InputModel(OperatorModel* op, unsigned int id, QUndoStack* undoStack, QObject * parent);
    
    /** Pushes a set color command on the undo stack. */
    void setColor(const QColor & color);
    
    /** Returns the current color. */
    const QColor & color() const { return m_color; }
    
    /** Pushes a set active command on the undo stack. */
    void setActive(bool active);
    
    /** Returns the current color. */
    bool active() const { return m_active; }
    
    /** Returns the current visualization type. */
    Visualization visualization() const { return m_visualization; }
    
    /** Pushes a set visualization command on the undo stack. */
    void setVisualization(Visualization visualization);
    
    /** Returns the operator model. */
    OperatorModel* op() const { return m_op; }
    
    /** Returns the ID of the operator input associated with this model. */
    unsigned int id() const { return m_id; }
    
public slots:
    /** Triggers an update of the operator name in all connected views. */
    void updateOperatorName(const QString & name);
    
signals:
    /** A property of the input model changed. */
    void changed(InputModel* model);
    
    /** The activation statu8s of the input changed. */
    void activeChanged(bool active);
    
    /** The color of the input changed. */
    void colorChanged(const QColor & color);
    
    /** The visualization type of the input changed. */
    void visualizationChanged(Visualization visualization);
    
private:
    /** Sets the activation status of the input. */
    void doSetActive(bool active);
    
    /** Sets the color of the input. */
    void doSetColor(const QColor & color);
    
    /** Sets the visualization type of the input. */
    void doSetVisualization(const Visualization & visualization);
    
    OperatorModel* m_op;
    unsigned int m_id;
    bool m_active;
    QColor m_color;
    Visualization m_visualization;
    QUndoStack* m_undoStack;
};

#endif // INPUTMODEL_H
