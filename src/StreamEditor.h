/* 
*  Copyright 2011 Matthias Fuchs
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

#ifndef STREAMEDITOR_H
#define STREAMEDITOR_H

#include "GraphicsView.h"
#include "StreamModel.h"

class ConnectionItem;
class StreamEditorScene;
class StreamModel;

/** 
 * \brief Stream editor widget.
 *
 * A stream editor is a widget which displays a stream editor scene.
 */
class StreamEditor : public GraphicsView
{
    Q_OBJECT

public:
    /** Constructs a stream editor. */
    explicit StreamEditor(QWidget *parent = 0);
    
    /** Returns the scene of the stream editor. */
    StreamEditorScene* streamEditorScene() const { return m_scene; }

    
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    
private:
    void startDrag();
    /** Sets the current center point of the view in scene coordinates. */
    void setCenter(const QPointF center);
    /** Returns the current center point of the view in scene coordinates. */
    QPointF getCenter() const {return m_currentCenter;}
    
    QPoint m_startPos;
    QPoint m_lastPanPos;
    QPointF m_currentCenter;
    OperatorModel* m_targetOp;
    unsigned int m_inputId;
    StreamEditorScene* m_scene;
};

#endif // STREAMEDITOR_H
