/* 
*  Copyright 2012 Thomas Fidler
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

#ifndef DATACONVERTER_H
#define DATACONVERTER_H

#include <memory>

class QVariant;

namespace stromx
{
    namespace runtime
    { 
        class Data;
        class DataRef;
        class Parameter;
    }
}

/** \brief Convenience functions to translate and compare QVariants and stromx data. */
class DataConverter
{   
public:
    /** 
     * Converts the stromx data object \c data of the parameter \c param to a QVariant object.
     * The type of the returned QVariant depends on \c role.
     */
    static QVariant toQVariant(const stromx::runtime::Data & data, const stromx::runtime::Parameter & param, int role);
    
    /**
     * Converts the input \c variant to the type defined by the parameter \c param and writes the
     * result to a new object of the correct stromx data type.
     */
    static stromx::runtime::DataRef toStromxData(const QVariant & variant, const stromx::runtime::Parameter & param);
    
    /**
     * Returns true if the data if \c newValue is of a type derived from \c targetValue and
     * \c newValue equals \c targetValue .
     */
    static bool stromxDataEqualsTarget(const stromx::runtime::Data & newValue, const stromx::runtime::Data & targetValue);
    
};

#endif // DATACONVERTER_H