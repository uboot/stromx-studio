#include "StreamModelTest.h"

#include <QtTest/QtTest>
#include <QUndoStack>
#include <stromx/core/DirectoryFileInput.h>
#include <stromx/core/ZipFileInput.h>

#include "../Exception.h"
#include "../OperatorLibraryModel.h"
#include "../StreamModel.h"

StreamModelTest::StreamModelTest()
  : m_undoStack(new QUndoStack(this)),
    m_operatorLibraryModel(new OperatorLibraryModel(this))
{
    try
    {
#ifdef UNIX
    m_operatorLibraryModel->loadPackage("libstromx_test");
#endif // UNIX
    
#ifdef WIN32
    m_operatorLibraryModel->loadPackage("stromx_test");
#endif // WIN32
    }
    catch(LoadPackageFailed&)
    {
    }
}

void StreamModelTest::testDefaultConstructor()
{
    new StreamModel(m_undoStack, m_operatorLibraryModel, this);
}

void StreamModelTest::testFileConstructorCamera()
{
    stromx::core::ZipFileInput input("camera.stromx");
    StreamModel* model = new StreamModel(input, "stream", m_undoStack, m_operatorLibraryModel, this);
    model->readStudioData(input, "stream");
}

void StreamModelTest::testFileConstructorConnector()
{
    stromx::core::DirectoryFileInput input(".");
    new StreamModel(input, "connector", m_undoStack, m_operatorLibraryModel, this);
}

void StreamModelTest::testFileConstructorExtraParameter()
{
    stromx::core::ZipFileInput input("extra_parameter.stromx");
    try
    {
        new StreamModel(input, "stream", m_undoStack, m_operatorLibraryModel, this);
        QFAIL("ReadStreamFailed not thrown");
    }
    catch(ReadStreamFailed &)
    {
    }
}




