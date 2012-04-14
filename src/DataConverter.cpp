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
#include "DataConverter.h"

#include <QVariant>
#include <QString>
#include <stromx/core/Image.h>
#include <stromx/core/Parameter.h>

QVariant DataConverter::toQVariant(const stromx::core::Data& data, const stromx::core::Parameter& param, int role)
{
    if(role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();
    
    QVariant variant;
    QString dataString;
    
    if (data.isVariant(stromx::core::DataVariant::NONE))
    {
        return QString("None");
    }
    
    if (data.isVariant(stromx::core::DataVariant::TRIGGER))
    {
        return QString("Trigger");
    }
    
    if (data.isVariant(stromx::core::DataVariant::BOOL))
    {
        const stromx::core::Bool & boolData = stromx::core::data_cast<const stromx::core::Bool&>(data);
        if(bool(boolData))
        {
            return QString(QObject::tr("true"));
        }
        else
        {
            return QString(QObject::tr("false"));
        }
    }
    
    if (data.isVariant(stromx::core::DataVariant::ENUM))
    {
        const stromx::core::Enum& value = stromx::core::data_cast<const stromx::core::Enum &>(data);
        unsigned int intValue = (unsigned int)(value);
        const std::vector<stromx::core::EnumDescription> & vectorEnumDesc = param.descriptions();
        for(std::vector<stromx::core::EnumDescription>::const_iterator iter_enumDesc = vectorEnumDesc.begin();
            iter_enumDesc != vectorEnumDesc.end();
            ++iter_enumDesc)
            {
                if (intValue == iter_enumDesc->value())
                    return QString::fromStdString(iter_enumDesc->description());
            }
        
        return QString(QObject::tr("<Unknown ENUM>"));
    }
    
    if (data.isVariant(stromx::core::DataVariant::INT_8))
    {
        const stromx::core::Int8 & int8Data = stromx::core::data_cast<const stromx::core::Int8&>(data);
        dataString.setNum((int)(int8Data));
        return dataString;
    }
    
    if (data.isVariant(stromx::core::DataVariant::UINT_8))
    {
        const stromx::core::UInt8 & uintData = stromx::core::data_cast<const stromx::core::UInt8&>(data);
        dataString.setNum((unsigned int)(uintData));
        return dataString;
    }
    
    if (data.isVariant(stromx::core::DataVariant::INT_16))
    {
        const stromx::core::Int16 & int16Data = stromx::core::data_cast<const stromx::core::Int16&>(data);
        dataString.setNum((int)(int16Data));
        return dataString;
    }
    
    if (data.isVariant(stromx::core::DataVariant::UINT_16))
    {
        const stromx::core::UInt16 & uint16Data = stromx::core::data_cast<const stromx::core::UInt16&>(data);
        dataString.setNum((unsigned int)(uint16Data));
        return dataString;
    }
    
    if (data.isVariant(stromx::core::DataVariant::INT_32))
    {
        const stromx::core::Int32 & int32Data = stromx::core::data_cast<const stromx::core::Int32&>(data);
        dataString.setNum((int)(int32Data));
        return dataString;
    }
    
    if (data.isVariant(stromx::core::DataVariant::UINT_32))
    {
        const stromx::core::UInt32 & uint32Data = stromx::core::data_cast<const stromx::core::UInt32&>(data);
        dataString.setNum((unsigned int)(uint32Data));
        return dataString;
    }
    
    if (data.isVariant(stromx::core::DataVariant::FLOAT))
    {
        const stromx::core::Float & floatData = stromx::core::data_cast<const stromx::core::Float&>(data);
        dataString.setNum((double)(floatData));
        return dataString;
    }
    
    if (data.isVariant(stromx::core::DataVariant::DOUBLE))
    {
        const stromx::core::Double & doubleData = stromx::core::data_cast<const stromx::core::Double&>(data);
        dataString.setNum((double)(doubleData));
        return dataString;
    }
    
    if (data.isVariant(stromx::core::DataVariant::IMAGE))
    {
        QString label1;
        QString label2;
        const stromx::core::Image & imageData = stromx::core::data_cast<const stromx::core::Image&>(data);
        return QString(QObject::tr("Height: %1 | Width: %2")).arg(label1.setNum(imageData.height()),label2.setNum(imageData.height()));
    }
    
    if (data.isVariant(stromx::core::DataVariant::DATA))
    {
        return QString("Data");
    }
    
    else
        return QString("<Unknown>");

}

std::auto_ptr<stromx::core::Data> DataConverter::toStromxData(QVariant& variant, stromx::core::Parameter& description)
{

}
