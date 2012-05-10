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

#ifndef LIMITUNDOSTACK_H
#define LIMITUNDOSTACK_H

#include <QUndoStack>

/** 
 * \brief Undo stack with a limit of undoable actions.
 *
 * If the \em limit of this undo stack is activated it prevents the undo
 * stack from undoing commands which where pushed prior to the activiation
 * of the limit.
 * In stromx-studio the limit is activated if a stream is started. After the
 * activation no changes which happened before the start of the stream can 
 * be undone. This prevents e.g. the removal of an previously added operator
 * while the stream is active.
 */
class LimitUndoStack : public QUndoStack
{
    Q_OBJECT
    
public:
    LimitUndoStack(QObject* parent = 0);
    
    QAction* createLimitUndoAction(QObject* parent, const QString & prefix = QString());
    
    /** 
     * Stops the undo stack from undoing commands with an index smaller or equal
     * to the current index.
     */ 
    void activateLimit();
    
    /** Deactivates the limit, i.e. all undos are allowed. */
    void deactivateLimit();
    
signals:
    void undoActionEnabledChanged(bool enabled);
    
private slots:
    void handleCanUndoChanged(bool canUndo);
    void handleUndoIndexChanged(int index);
   
private:
    int m_currentUndoLimit;
};

#endif // LIMITUNDOSTACK_H
