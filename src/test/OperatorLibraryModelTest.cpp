#include "OperatorLibraryModelTest.h"

#include "Config.h"
#include "../Exception.h"
#include "../OperatorData.h"
#include "../OperatorLibraryModel.h"
#include <stromx/core/Factory.h>
#include <stromx/core/Operator.h>
#include <QModelIndex>


CPPUNIT_TEST_SUITE_REGISTRATION (OperatorLibraryModelTest);

void OperatorLibraryModelTest::setUp()
{
    m_model = new OperatorLibraryModel();
    m_model->resetLibraries();
    
    QString library(STROMX_BASE_LIBRARY);
    CPPUNIT_ASSERT_NO_THROW(m_model->loadLibrary(library));
}

void OperatorLibraryModelTest::tearDown()
{
    delete m_model;
}

void OperatorLibraryModelTest::testLoadLibrary()
{
    m_model->resetLibraries();
    
    QString library(STROMX_BASE_LIBRARY);
    CPPUNIT_ASSERT_NO_THROW(m_model->loadLibrary(library));
    
    CPPUNIT_ASSERT_EQUAL((unsigned int)(12), (unsigned int)(m_model->m_factory->availableOperators().size()));
    CPPUNIT_ASSERT_EQUAL(11, m_model->rowCount(m_model->createIndex(1, 0, 1)));
}

void OperatorLibraryModelTest::testLoadLibraryDoubleData()
{
    QString library(STROMX_BASE_LIBRARY);
    CPPUNIT_ASSERT_THROW(m_model->loadLibrary(library), LoadLibraryFailed);
}

void OperatorLibraryModelTest::testLoadLibraryWrongFile()
{
    QString library("/stupid_path_to/wrong_library.so");
    
    CPPUNIT_ASSERT_THROW(m_model->loadLibrary(library), LoadLibraryFailed);
}

void OperatorLibraryModelTest::testRowCount()
{
    QModelIndex index = QModelIndex();
    CPPUNIT_ASSERT_EQUAL(2, m_model->rowCount(index));
    
    index = m_model->createIndex(0, 0, 0);
    CPPUNIT_ASSERT_EQUAL(1, m_model->rowCount(index));
    
    index = m_model->createIndex(1, 0, 1);
    CPPUNIT_ASSERT_EQUAL(11, m_model->rowCount(index));
    
    index = m_model->createIndex(0, 0, 3);
    CPPUNIT_ASSERT_EQUAL(0, m_model->rowCount(index));
}

void OperatorLibraryModelTest::testData()
{
    QVariant data = m_model->data(m_model->createIndex(0, 0, 0), Qt::DisplayRole);
    CPPUNIT_ASSERT_EQUAL(std::string("Core"), data.toString().toStdString());
    
    data = m_model->data(m_model->createIndex(1, 0, 1), Qt::DisplayRole);
    CPPUNIT_ASSERT_EQUAL(std::string("Base"), data.toString().toStdString());
    
    data = m_model->data(m_model->createIndex(0, 0, 2), Qt::DisplayRole);
    CPPUNIT_ASSERT_EQUAL(std::string("Dump"), data.toString().toStdString());
    
    data = m_model->data(m_model->createIndex(1, 0, 3), Qt::DisplayRole);
    CPPUNIT_ASSERT_EQUAL(std::string("Buffer"), data.toString().toStdString());
}

void OperatorLibraryModelTest::testIndex()
{
    // second package
    QModelIndex parent = QModelIndex();
    QModelIndex index = m_model->index(1, 0, parent);
    CPPUNIT_ASSERT_EQUAL(1, index.row());
    CPPUNIT_ASSERT_EQUAL(qint64(1), index.internalId());
    
    // first operator of first package
    parent = m_model->createIndex(0, 0, 0);
    index = m_model->index(0, 0, parent);
    CPPUNIT_ASSERT_EQUAL(0, index.row());
    CPPUNIT_ASSERT_EQUAL(qint64(2), index.internalId());
    
    // third operator of second package
    parent = m_model->createIndex(1, 0, 1);
    index = m_model->index(2, 0, parent);
    CPPUNIT_ASSERT_EQUAL(2, index.row());
    CPPUNIT_ASSERT_EQUAL(qint64(3), index.internalId());
}

void OperatorLibraryModelTest::testParent()
{
    // second package
    QModelIndex index = m_model->createIndex(0, 0, 1);
    QModelIndex parent = m_model->parent(index);
    CPPUNIT_ASSERT(! parent.isValid());
    
    // first operator of first package
    index = m_model->createIndex(0, 0, 2);
    parent = m_model->parent(index);
    CPPUNIT_ASSERT_EQUAL(0, parent.row());
    CPPUNIT_ASSERT_EQUAL(qint64(0), parent.internalId());
    
    // second operator of second package
    index = m_model->createIndex(1, 0, 3);
    parent = m_model->parent(index);
    CPPUNIT_ASSERT_EQUAL(1, parent.row());
    CPPUNIT_ASSERT_EQUAL(qint64(1), parent.internalId());
}

void OperatorLibraryModelTest::testIsOperator()
{
    // second package
    QModelIndex index = m_model->createIndex(0, 0, 1);
    CPPUNIT_ASSERT_EQUAL(false, m_model->isOperator(index));
    
    // first operator of first package
    index = m_model->createIndex(0, 0, 2);
    CPPUNIT_ASSERT_EQUAL(true, m_model->isOperator(index));
}

void OperatorLibraryModelTest::testNewOperator()
{
    // wrong operator
    stromx::core::Operator* op = 0;
    OperatorData opData1("NoPackage", "NoOperator");
    CPPUNIT_ASSERT_NO_THROW(op = m_model->newOperator(&opData1));
    CPPUNIT_ASSERT(! op);
    
    // existing operator
    OperatorData opData2("Base", "Buffer");
    CPPUNIT_ASSERT_NO_THROW(op = m_model->newOperator(&opData2));
    CPPUNIT_ASSERT(op);
    CPPUNIT_ASSERT_EQUAL(std::string("Base"), op->info().package());
    CPPUNIT_ASSERT_EQUAL(std::string("Buffer"), op->info().type());
}

void OperatorLibraryModelTest::testNewOperatorData()
{
    // second package
    OperatorData* opData = 0;
    QModelIndex index = m_model->createIndex(0, 0, 1);
    CPPUNIT_ASSERT_NO_THROW(opData = m_model->newOperatorData(index));
    CPPUNIT_ASSERT(! opData);
    
    // second operator of second package
    index = m_model->createIndex(1, 0, 3);
    CPPUNIT_ASSERT_NO_THROW(opData = m_model->newOperatorData(index));
    CPPUNIT_ASSERT(opData);
    CPPUNIT_ASSERT_EQUAL(std::string("Base"), opData->package().toStdString());
    CPPUNIT_ASSERT_EQUAL(std::string("Buffer"), opData->type().toStdString());
    
    delete opData;
}

