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

#include "Common.h"
#include <QVariant>
#include <QString>
#include <QStringList>
#include <stromx/core/Image.h>
#include <stromx/core/Parameter.h>
#include <stromx/core/Trigger.h>

QVariant DataConverter::toQVariant(const stromx::core::Data& data, const stromx::core::Parameter& param, int role)
{
    try
    {
        QVariant variant;
        QString dataString;
        
        if (data.isVariant(stromx::core::DataVariant::NONE))
        {
            if(role == Qt::DisplayRole)
                return QString("None");
        }
        
        if (data.isVariant(stromx::core::DataVariant::TRIGGER))
        {
            if(role == Qt::DisplayRole)
                return QString("Trigger");
            
            if(role == TriggerRole)
                return QString("Trigger");
        }
        
        if (data.isVariant(stromx::core::DataVariant::BOOL))
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
            {
                const stromx::core::Bool & boolData = stromx::core::data_cast<const stromx::core::Bool&>(data);
                return bool(boolData);
            }
        }
        
        if (data.isVariant(stromx::core::DataVariant::ENUM))
        {
            const stromx::core::Enum& value = stromx::core::data_cast<const stromx::core::Enum &>(data);
            unsigned int intValue = (unsigned int)(value);
            const std::vector<stromx::core::EnumDescription> & vectorEnumDesc = param.descriptions();
            
            if(role == Qt::DisplayRole)
            {
                for(std::vector<stromx::core::EnumDescription>::const_iterator iter_enumDesc = vectorEnumDesc.begin();
                    iter_enumDesc != vectorEnumDesc.end();
                    ++iter_enumDesc)
                {
                    if (intValue == iter_enumDesc->value())
                        return QString::fromStdString(iter_enumDesc->description());
                }
                return QString(QObject::tr("<Unknown ENUM>"));
            }
            else if(role == Qt::EditRole)
            {
                int choice = 0;
                for(std::vector<stromx::core::EnumDescription>::const_iterator iter_enumDesc = vectorEnumDesc.begin();
                    iter_enumDesc != vectorEnumDesc.end();
                    ++iter_enumDesc)
                {
                    if (intValue == iter_enumDesc->value())
                        return choice;
                    ++choice;
                }
                return -1;
            } 
            else if(role == ChoicesRole)
            {
                QStringList choices;
                for(std::vector<stromx::core::EnumDescription>::const_iterator iter_enumDesc = vectorEnumDesc.begin();
                    iter_enumDesc != vectorEnumDesc.end();
                    ++iter_enumDesc)
                {
                    choices.append(QString::fromStdString(iter_enumDesc->description()));
                }
                return choices;
            }
        }
        
        if (data.isVariant(stromx::core::DataVariant::INT_8))
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
            {
                const stromx::core::Int8 & int8Data = stromx::core::data_cast<const stromx::core::Int8&>(data);
                return int(int8Data);
            }
        }
        
        if (data.isVariant(stromx::core::DataVariant::UINT_8))
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
            {
                const stromx::core::UInt8 & uint8Data = stromx::core::data_cast<const stromx::core::UInt8&>(data);
                return int(uint8Data);
            }
        }
        
        if (data.isVariant(stromx::core::DataVariant::INT_16))
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
            {
                const stromx::core::Int16 & int16Data = stromx::core::data_cast<const stromx::core::Int16&>(data);
                return int(int16Data);
            }
        }
        
        if (data.isVariant(stromx::core::DataVariant::UINT_16))
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
            {
                const stromx::core::UInt16 & uint16Data = stromx::core::data_cast<const stromx::core::UInt16&>(data);
                return int(uint16Data);
            }
        }
        
        if (data.isVariant(stromx::core::DataVariant::INT_32))
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
            {
                const stromx::core::Int32 & int32Data = stromx::core::data_cast<const stromx::core::Int32&>(data);
                return int(int32Data);
            }
        }
        
        if (data.isVariant(stromx::core::DataVariant::UINT_32))
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
            {
                const stromx::core::UInt32 & uint32Data = stromx::core::data_cast<const stromx::core::UInt32&>(data);
                return int(uint32Data);
            }
        }
        
        if (data.isVariant(stromx::core::DataVariant::FLOAT))
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
            {
                const stromx::core::Float & floatData = stromx::core::data_cast<const stromx::core::Float&>(data);
                return double(floatData);
            }
        }
        
        if (data.isVariant(stromx::core::DataVariant::DOUBLE))
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
            {
                const stromx::core::Double & doubleData = stromx::core::data_cast<const stromx::core::Double&>(data);
                return double(doubleData);
            }
        }
        
        if (data.isVariant(stromx::core::DataVariant::IMAGE))
        {
            if(role == Qt::DisplayRole)
            {
                QString label1;
                QString label2;
                const stromx::core::Image & imageData = stromx::core::data_cast<const stromx::core::Image&>(data);
                return QString(QObject::tr("Height: %1 | Width: %2")).arg(label1.setNum(imageData.height()),label2.setNum(imageData.height()));
            }
        }
        
        if (data.isVariant(stromx::core::DataVariant::DATA))
        {
            if(role == Qt::DisplayRole)
            {
                return QString("Data");
            }
        }
        
        return QVariant();
    }
    catch(stromx::core::BadCast&)
    {
        return QVariant();
    }  
}

std::auto_ptr<stromx::core::Data> DataConverter::toStromxData(const QVariant& variant, const stromx::core::Parameter& param)
{
    if(param.variant().isVariant(stromx::core::DataVariant::TRIGGER))
    {
        if(variant.type() == QVariant::Bool && variant.toBool())
            return std::auto_ptr<stromx::core::Data>(new stromx::core::Trigger());
    }
    
    if(param.variant().isVariant(stromx::core::DataVariant::BOOL))
    {
        if(variant.type() == QVariant::Bool)
            return std::auto_ptr<stromx::core::Data>(new stromx::core::Bool(variant.toBool()));
    }
    
    if(param.variant().isVariant(stromx::core::DataVariant::INT_8))
    {
        if(variant.type() == QVariant::Int)
            return std::auto_ptr<stromx::core::Data>(new stromx::core::Int8(variant.toInt()));
    }
    
    if(param.variant().isVariant(stromx::core::DataVariant::UINT_8))
    {
        if(variant.type() == QVariant::Int)
            return std::auto_ptr<stromx::core::Data>(new stromx::core::UInt8(variant.toInt()));
    }
    
    if(param.variant().isVariant(stromx::core::DataVariant::INT_16))
    {
        if(variant.type() == QVariant::Int)
            return std::auto_ptr<stromx::core::Data>(new stromx::core::Int16(variant.toInt()));
    }
    
    if(param.variant().isVariant(stromx::core::DataVariant::UINT_16))
    {
        if(variant.type() == QVariant::Int)
            return std::auto_ptr<stromx::core::Data>(new stromx::core::UInt16(variant.toInt()));
    }
    
    if(param.variant().isVariant(stromx::core::DataVariant::INT_32))
    {
        if(variant.type() == QVariant::Int)
            return std::auto_ptr<stromx::core::Data>(new stromx::core::Int32(variant.toInt()));
    }
    
    if(param.variant().isVariant(stromx::core::DataVariant::UINT_32))
    {
        if(variant.type() == QVariant::Int)
            return std::auto_ptr<stromx::core::Data>(new stromx::core::UInt32(variant.toInt()));
    }
    
    if(param.variant().isVariant(stromx::core::DataVariant::ENUM))
    {
        if(variant.type() == QVariant::Int)
        {  
            // find the description whose value equals the input data
            int index = variant.toInt();
            if((unsigned int)(index) < param.descriptions().size())
            {    
                unsigned int value = param.descriptions()[index].value();
                return std::auto_ptr<stromx::core::Data>(new stromx::core::Enum(value));
            }
        }
    }
    
    if(param.variant().isVariant(stromx::core::DataVariant::FLOAT))
    {
        if(variant.type() == QVariant::Double)
            return std::auto_ptr<stromx::core::Data>(new stromx::core::Float(variant.toDouble()));
    }
    
    if(param.variant().isVariant(stromx::core::DataVariant::DOUBLE))
    {
        if(variant.type() == QVariant::Double)
            return std::auto_ptr<stromx::core::Data>(new stromx::core::Double(variant.toDouble()));
    }
    
    if(param.variant().isVariant(stromx::core::DataVariant::IMAGE))
    {
        if(variant.type() == QVariant::Image)
            return std::auto_ptr<stromx::core::Data>(); // allocate stromx image from variant
    }
    
    return std::auto_ptr<stromx::core::Data>();
}
