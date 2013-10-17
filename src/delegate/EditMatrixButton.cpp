#include "delegate/EditMatrixButton.h"

EditMatrixButton::EditMatrixButton(const Matrix & matrix, QWidget* parent)
  : QPushButton(parent),
    m_matrix(matrix)
{
    setText(tr("Edit..."));
    connect(this, SIGNAL(clicked(bool)), this, SLOT(openEditor()));
}

void EditMatrixButton::openEditor()
{
    emit finishedEditing();
}

void EditMatrixButton::setNumColumns(const int cols)
{

}

void EditMatrixButton::setNumRows(const int rows)
{

}
