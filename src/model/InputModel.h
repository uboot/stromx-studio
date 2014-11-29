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

#include <QColor>
#include <QObject>

class QJsonObject;
class QUndoStack;
class ObserverModel;
class ObserverTreeModel;
class OperatorModel;

#include "visualization/VisualizationState.h"

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
    
    friend class SetVisualizationStateCmd;
    friend QDataStream & operator>>(QDataStream & stream, InputModel * model);
    friend QDataStream & readVersion01(QDataStream & stream, InputModel * model);
    
public:    
    /** Constructs an input model for the input \c id of the operator \c op. */
    InputModel(OperatorModel* op, unsigned int id, QUndoStack* undoStack, QObject * parent);
    
    /** Returns the observer model this input currently belongs to. */
    ObserverModel* parentModel() const { return m_parentModel; }
    
    /** Sets the observer model this input currently belongs to. */
    void setParentModel(ObserverModel* const parentModel) { m_parentModel = parentModel; }
    
    /** Returns the visualization state. */
    VisualizationState visualizationState() const { return m_visualizationState; }
    
    /** Pushes a set visualization state command on the undo stack. */
    void setVisualizationState(const VisualizationState & state);
    
    /** Returns the operator model. */
    OperatorModel* op() const { return m_op; }
    
    /** Returns the ID of the operator input associated with this model. */
    unsigned int id() const { return m_id; }
    
    /** Returns the documentation title of the operator input associated with this model. */
    QString docTitle() const;
    
#ifndef STROMX_STUDIO_QT4
    /** Writes the input to \c json. */
    void write(QJsonObject & json) const;
    
    /** Replaces the content of the input by the data in \c json. */
    void read(const QJsonObject & json);
#endif
    
public slots:
    /** Triggers an update of the operator name in all connected views. */
    void updateOperatorName(const QString & name);
    
signals:
    /** A property of the input model changed. */
    void changed(InputModel* model);
    
    /** The visualization state of the input changed. */
    void visualizationStateChanged(VisualizationState state);
    
private:
    /** Sets the visualization type of the input. */
    void doSetVisualizationState(const VisualizationState & state);
    
    OperatorModel* m_op;
    unsigned int m_id;
    VisualizationState m_visualizationState;
    QUndoStack* m_undoStack;
    ObserverModel* m_parentModel;
};

QDataStream & operator<<(QDataStream & stream, const InputModel * model);
QDataStream & operator>>(QDataStream & stream, InputModel * model);
QDataStream & readVersion01(QDataStream & stream, InputModel * model);

#endif // INPUTMODEL_H
