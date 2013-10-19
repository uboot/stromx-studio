#include "delegate/EditMatrixButton.h"

#include "widget/MatrixEditor.h"


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
    emit finishedEditing();
}

const Matrix& EditMatrixButton::matrix()
{
    return m_editor->matrix();
}

