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
class OperatorModel;

class InputModel : public QObject
{
    Q_OBJECT
    
public:
    InputModel(OperatorModel* op, unsigned int id, QUndoStack* undoStack, QObject * parent);
    
    void setColor(const QColor & color);
    const QColor & color() const { return m_color; }
    
    OperatorModel* op() const { return m_op; }
    unsigned int id() const { return m_id; }
    
public slots:
    void updateOperatorName(const QString & name);
    
signals:
    /** A property of the input model changed. */
    void changed(InputModel* model);
    
    /** The color of the input changed. */
    void colorChanged(const QColor & color);
    
private:
    OperatorModel* m_op;
    unsigned int m_id;
    QColor m_color;
    QUndoStack* m_undoStack;
};

#endif // INPUTMODEL_H
