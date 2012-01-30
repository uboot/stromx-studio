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

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

class Exception : public std::exception
{
}; 

class LoadLibraryFailed : public Exception
{
public:
    LoadLibraryFailed() {}
};

class ReadStreamFailed : public Exception
{
public:
    ReadStreamFailed() {}
};

class ReadModelFailed : public Exception
{
public:
    ReadModelFailed() {}
};

class WriteStreamFailed : public Exception
{
public:
    WriteStreamFailed() {}
};

#endif // EXCEPTION_H
