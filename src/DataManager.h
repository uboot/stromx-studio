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

#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>

#include "OperatorModel.h"

class AbstractDataVisualizer;
class InputModel;
class ObserverModel;

namespace stromx
{
    namespace core
    {
        class DataContainer;
    }
}

/** \brief Manager of the communication between an observer model and a data visualizer
 * 
 * A data manager contains references to an observer model and a data visualizer.
 * It connects to signals of the observer model and the input models of the observer.
 * Whenever the observer (i.e. inputs are added, moved or removed) or properties 
 * of an input change the respective commands of the data visualizer are called.
 * In particular if data is observed by an input model the data manager updates
 * it in the visualizer.
 */
class DataManager : public QObject
{
    Q_OBJECT
    
public:
    /** 
     * Constructs a data manager which routes the signals it receives from
     * \c observer to \c visualizer.
     */
    DataManager(ObserverModel* observer, AbstractDataVisualizer* visualizer, QObject* parent);
    
private slots:
    
    /** 
     * Removes the input layer from the visualizer and disconnects all connections from the input.
     * Moreover, the connection from the operator of the input are disconnected if no
     * other input requires them.
     */
    void removeInputLayer(InputModel* input, int pos);
    
    /** Moves the input layer in the visualizer. */
    void moveInputLayer(InputModel* input, int srcPos, int destPos);
    
    /**
     * Connects to the input model and adds an input layer to the visualizer.
     * Moveover, if necessary a connection from the operator to the data manager
     * is established.
     */
    void addInputLayer(InputModel* input, int pos);
    
    /**
     * Updates the data of all layers which correspond to this input.
     * The input is specified by its type, ID and operator (the sender of the signal).
     */
    void updateLayerData(OperatorModel::ConnectorType type, unsigned int id, stromx::core::DataContainer data);

    /**
     * Updates the properties (color, alpha, etc.) of all layers which correspond to this input.
     */
    void updateInputProperties(InputModel* input);
    
private:
    /** 
     * Connects to the operator of the input if the connections has not yet been established
     * because of another input of the same operator.
     */
    void connectInput(InputModel* input);
    
    /**
     * Disconnects from the operator of the input if the connection is not required
     * by another input of the same operator.
     */
    void disconnectInput(InputModel* input);
    
    ObserverModel* m_observer;
    AbstractDataVisualizer* m_visualizer;
    QList<InputModel*> m_inputs;
};

#endif // DATAMANAGER_H
