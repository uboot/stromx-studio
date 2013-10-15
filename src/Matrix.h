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

#ifndef MATRIX_H
#define MATRIX_H

#include <QVariant>

#include <stromx/runtime/MatrixWrapper.h>
#include <stromx/runtime/Version.h>

/** \brief Concrete stromx matrix class. */
class Matrix : public stromx::runtime::MatrixWrapper
{
public:
    Matrix() : m_data(0) {}
    Matrix(const Matrix & matrix);
    Matrix(const unsigned int rows, const unsigned int cols, const ValueType valueType);
    
    const stromx::runtime::Version & version() const { return VERSION; }
    const std::string & type() const { return TYPE; }
    const std::string & package() const { return PACKAGE; }
    
    Data* clone() const { return new Matrix(); }
    
    const Matrix & operator=(const Matrix & matrix);
    
protected:
    void allocate(const unsigned int rows, const unsigned int cols, const ValueType valueType);
    
private:
    static const std::string TYPE;
    static const std::string PACKAGE;
    static const stromx::runtime::Version VERSION;
    uint8_t* m_data;
};

Q_DECLARE_METATYPE(Matrix)

#endif // MATRIX_H
