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

#ifndef UNDOSTACKACTION_H
#define UNDOSTACKACTION_H

#include <QAction>

/** \brief Action representing an undo or redo action. */
class UndoStackAction : public QAction
{
    Q_OBJECT
    
public:
    /** 
     * Constructs an undo stack action. The text of this action
     * is always appended to \c prefix.
     */
    UndoStackAction(const QString & prefix, QObject * parent = 0);
    
public slots:
    /** Sets the text of the action to the prefix followed by \c text. */
    void setUndoText(const QString & text);
    
private:
    QString m_prefix;
};

#endif // UNDOSTACKACTION_H
