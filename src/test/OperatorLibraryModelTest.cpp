#include "OperatorLibraryModelTest.h"

#include "../OperatorLibraryModel.h"


CPPUNIT_TEST_SUITE_REGISTRATION (OperatorLibraryModelTest);

void OperatorLibraryModelTest::setUp()
{
    m_model = new OperatorLibraryModel();
}

void OperatorLibraryModelTest::tearDown()
{
    delete m_model;
}

void OperatorLibraryModelTest::testRowCount()
{

}
