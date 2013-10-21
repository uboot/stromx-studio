#include "delegate/EditMatrixButton.h"

#include "widget/MatrixEditor.h"
#include <boost/graph/graph_concepts.hpp>

EditMatrixButton::EditMatrixButton(const Matrix & matrix, const int rows,
                                   const int cols, QWidget* parent)
  : QPushButton(parent),
    m_editor(0)
{
    setText(tr("Edit..."));
    m_editor = new MatrixEditor(matrix, rows, cols, this);
    
    connect(this, SIGNAL(clicked(bool)), this, SLOT(openEditor()));
}

void EditMatrixButton::openEditor()
{
    m_editor->exec();
    emit finishedEditing();
}

const Matrix& EditMatrixButton::matrix()
{
    return m_editor->matrix();
}

