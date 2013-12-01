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

#ifndef SETSTREAMSETTINGSCMD_H
#define SETSTREAMSETTINGSCMD_H

#include <QMap>
#include <QUndoCommand>

class StreamModel;

/** \brief Sets the settings of the stream model. */
class SetStreamSettingsCmd : public QUndoCommand
{
public:
    SetStreamSettingsCmd(StreamModel* model, 
        const QMap<QString, QVariant> & newSettings, QUndoCommand* parent = 0);
    
    virtual void undo();
    virtual void redo();
    
private:
    StreamModel* m_model;
    QMap<QString, QVariant> m_newSettings;
    QMap<QString, QVariant> m_oldSettings;
};

#endif // SETSTREAMSETTINGSCMD_H