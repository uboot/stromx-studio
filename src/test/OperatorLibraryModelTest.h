/* 
*  Copyright 2011 Matthias Fuchs
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#ifndef OPERATORLIBRARYMODELTEST_H
#define OPERATORLIBRARYMODELTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

class OperatorLibraryModel;

class OperatorLibraryModelTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (OperatorLibraryModelTest);
    CPPUNIT_TEST(testLoadLibraries);
    CPPUNIT_TEST(testLoadLibrariesWrongFile);
    CPPUNIT_TEST(testLoadLibrariesDoubleData);
    CPPUNIT_TEST(testRowCount);
    CPPUNIT_TEST(testParent);
    CPPUNIT_TEST(testIndex);
    CPPUNIT_TEST(testData);
    CPPUNIT_TEST_SUITE_END ();

public:
    OperatorLibraryModelTest() : m_model(0) {}
    
    void setUp();
    void tearDown();

protected:
    void testLoadLibraries();
    void testLoadLibrariesWrongFile();
    void testLoadLibrariesDoubleData();
    void testRowCount();
    void testParent();
    void testIndex();
    void testData();
        
private:
    OperatorLibraryModel* m_model;
};
        
#endif // OPERATORLIBRARYMODELTEST_H