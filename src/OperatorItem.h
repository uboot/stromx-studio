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

#ifndef OPERATORITEM_H
#define OPERATORITEM_H

#include <QGraphicsObject>

class QGraphicsRectItem;
class OperatorModel;

class OperatorItem : public QGraphicsObject
{
    Q_OBJECT
    
public:
    explicit OperatorItem(OperatorModel* op, QGraphicsItem * parent = 0);
    
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    
private:
    OperatorModel* m_op;
    QGraphicsRectItem* m_opRect;
};

#endif // OPERATORITEM_H
