/* 
*  Copyright 2011 xxx yyy
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

#ifndef STREAMEDITORSCENE_H
#define STREAMEDITORSCENE_H

#include <QGraphicsScene>

class QGraphicsSceneDragDropEvent;

class StreamEditorScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit StreamEditorScene(QObject *parent = 0);
    
    virtual void dropEvent(QGraphicsSceneDragDropEvent* event);
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent* event);
};

#endif // STREAMEDITORSCENE_H
