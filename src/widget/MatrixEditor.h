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

#ifndef MATRIXEDITOR_H
#define MATRIXEDITOR_H

#include <QDialog>

#include "Matrix.h"

class Matrix;
class QTableWidget;

class MatrixEditor : public QDialog
{   
    Q_OBJECT
    
public:
    explicit MatrixEditor(const Matrix& matrix, const int rows,
                          const int cols, QWidget* parent = 0);
    
    const Matrix & matrix() { return m_matrix; }
    
private:
    void fillTable();
    
    Matrix m_matrix;
    QTableWidget* m_table;
};

#endif // MATRIXEDITOR_H