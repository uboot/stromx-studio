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

#ifndef STREAMMODELTEST_H
#define STREAMMODELTEST_H

#include <QObject>

class QUndoStack;
class OperatorLibraryModel;

class StreamModelTest : public QObject
{
    Q_OBJECT
    
public:
    explicit StreamModelTest();
    
private slots:
    void testDefaultConstructor();
    void testFileConstructorCamera();
    void testFileConstructorConnector();
    
private:
    QUndoStack* m_undoStack;
    OperatorLibraryModel* m_operatorLibraryModel;
};

#endif // STREAMMODELTEST_H
