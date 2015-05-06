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

#ifndef CONNECTORDATAEVENT_H
#define CONNECTORDATAEVENT_H

#include <QEvent>
#include <stromx/runtime/DataContainer.h>

#include "Common.h"
#include "model/OperatorModel.h"

class ConnectorDataEvent : public QEvent
{
public:
    static const unsigned int TYPE = QEvent::User + ConnectorData;
    
    ConnectorDataEvent(OperatorModel::ConnectorType type, unsigned int id,
                       const stromx::runtime::ReadAccess & access);
    
    OperatorModel::ConnectorType type() const { return m_type; }
    unsigned int id() const { return m_id; }
    const stromx::runtime::ReadAccess & access() const { return m_access; }
    
private:
    OperatorModel::ConnectorType m_type;
    unsigned int m_id;
    stromx::runtime::ReadAccess m_access;
};

#endif // CONNECTORDATAEVENT_H
