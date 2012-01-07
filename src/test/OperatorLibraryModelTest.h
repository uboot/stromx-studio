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

#ifndef OPERATORLIBRARYMODELTEST_H
#define OPERATORLIBRARYMODELTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

class OperatorLibraryModel;

class OperatorLibraryModelTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (OperatorLibraryModelTest);
    CPPUNIT_TEST(testLoadLibrary);
    CPPUNIT_TEST(testLoadLibraryWrongFile);
    CPPUNIT_TEST(testLoadLibraryDoubleData);
    CPPUNIT_TEST(testRowCount);
    CPPUNIT_TEST(testParent);
    CPPUNIT_TEST(testIndex);
    CPPUNIT_TEST(testData);
    CPPUNIT_TEST(testIsOperator);
    CPPUNIT_TEST(testNewOperator);
    CPPUNIT_TEST_SUITE_END ();

public:
    OperatorLibraryModelTest() : m_model(0) {}
    
    void setUp();
    void tearDown();

protected:
    void testLoadLibrary();
    void testLoadLibraryWrongFile();
    void testLoadLibraryDoubleData();
    void testRowCount();
    void testParent();
    void testIndex();
    void testData();
    void testIsOperator();
    void testNewOperator();
        
private:
    OperatorLibraryModel* m_model;
};
        
#endif // OPERATORLIBRARYMODELTEST_H