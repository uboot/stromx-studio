#include "delegate/EditMatrixButton.h"

EditMatrixButton::EditMatrixButton(QWidget* parent)
  : QPushButton(parent)
{
    setText(tr("Edit..."));
    connect(this, SIGNAL(clicked(bool)), this, SLOT(openEditor()));
}

void EditMatrixButton::openEditor()
{
    emit finishedEditing();
}

void EditMatrixButton::setMatrix(const Matrix& matrix)
{
    m_matrix = matrix;
}
