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
    namespace core
    { 
        class Data;
        class Parameter;
    }
}

class DataConverter
{   
public:
    static QVariant toQVariant(const stromx::core::Data & data, const stromx::core::Parameter & param, int role);
    static std::auto_ptr<stromx::core::Data> toStromxData(const QVariant & variant, const stromx::core::Parameter & description);
};

#endif // DATACONVERTER_H