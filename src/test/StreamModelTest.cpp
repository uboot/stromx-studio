#include "StreamModelTest.h"

#include <QtTest/QtTest>
#include <QUndoStack>
#include <stromx/core/DirectoryFileInput.h>
#include <stromx/core/ZipFileInput.h>

#include "../OperatorLibraryModel.h"
#include "../StreamModel.h"

StreamModelTest::StreamModelTest()
  : m_undoStack(new QUndoStack(this)),
    m_operatorLibraryModel(new OperatorLibraryModel(this))
{
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




